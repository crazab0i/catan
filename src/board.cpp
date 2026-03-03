#include "board.hpp"

#include <iostream>
#include <algorithm>
#include <random>
#include <set>
#include <utility>

namespace Catan {

namespace Board {


//////////////////////////////////////////////////////////////////////////
//
//      STRUCTS
//
/////////////////////////////////////////////////////////////////////////

void Building::printBuilding() {
    std::cout << "owner_id: " << owner_id << "\n";
    std::cout << "building_type: ";
    switch (building_type) {
        case BuildingType::Road:        std::cout << "road"; break;
        case BuildingType::Settlement:  std::cout << "settlement"; break;
        case BuildingType::City:        std::cout << "city"; break;
        case BuildingType::Unbuilt:     std::cout << "unbuilt"; break;
    }
    std::cout << "\n";
}   


Point::Point(int id) : id(PointID(id)) {}
Edge::Edge(int id, PointID a, PointID b) : id(static_cast<EdgeID>(id)), a(a), b(b) {}
Tile::Tile(bool hasRobber, 
           int id, 
           int diceValue,
           TileType type) : hasRobber(hasRobber), id(id), diceValue(diceValue), type(type) {}

void Tile::printTile() const {
    std::cout << "========== tile id: " << static_cast<int>(id) << " ==========\n";
    std::cout << "type: " << TileType_to_string(type);
    std::cout << "\ndice value: " << diceValue;
    std::cout << "\nhas robber: " << hasRobber;
    std::cout << "\npoints: ";
    for (const auto point : tilePoints[id]) {
        std::cout << static_cast<int>(point) << ", ";
    }
    std::cout << "\n";
}

} // end Board namespace

//////////////////////////////////////////////////////////////////////////
//
//      Board Class
//
/////////////////////////////////////////////////////////////////////////

void GameBoard::_printTiles() const {
    for (const auto &tile : tiles) {
        tile.printTile();
    }
}

void GameBoard::_printPoints() const {
    for (const auto &point : points) {
        std::cout << "Point: " << static_cast<int>(point.id) << "\n";
        std::cout << "Adjacent Tiles: \n";
        for (auto adjacent_tile : point.tiles) {
            int tileID = static_cast<int>(adjacent_tile);
            if (tileID == Board::NON_TILE) {
                std::cout << "NON_TILE ";
            } else {
                std::cout << tileID << " ";
            }
        }
        std::cout << "\n";
        std::cout << "Adjacent Edges: \n";
        for (auto adjacent_edge : point.edges) {
            int edgeID = static_cast<int>(adjacent_edge);
            if (edgeID == Board::NON_EDGE) {
                std::cout << "NON_EDGE ";
            } else {
                std::cout << edgeID << " ";
            }
        }
        std::cout << "\n";
    }
}   

void GameBoard::_printEdges() const {
    for (auto edge : edges) {
        std::cout << "Edge: " <<  static_cast<int>(edge.id) << "\n";
        std::cout << "a: " << static_cast<int>(edge.a) << " , b: " << static_cast<int>(edge.b) << "\n"; 
        for (auto adjacent_edge : edge.edges) {
            int edgeID = static_cast<int>(adjacent_edge);
            if (edgeID == Board::NON_EDGE) {
                std::cout << "NON_EDGE ";
            } else {
                std::cout << edgeID << " ";
            }
        }
        std::cout << "\n";
    }

}

void GameBoard::createBoard() {
    std::vector<Board::TileType> types;
    types.reserve(Board::NUM_TILES);

    for (auto [type, count] : Board::tileCounts) {
        types.insert(types.end(), count, type);
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(begin(types), end(types), gen);

    const std::vector<int> dice_values = {5, 2, 6, 3, 8, 10, 9, 12, 11, 4, 8, 10, 9, 4, 5, 6, 3, 11};
    auto dice_it = begin(dice_values);

    tiles.clear();
    tiles.reserve(Board::NUM_TILES);


    constexpr int NUM_ROLL_POSSIBILITES = 11;
    tilesDieIndex.resize(NUM_ROLL_POSSIBILITES);

    for (size_t i = 0; i < types.size(); ++i) {
        
        if (types[i] == Board::TileType::Desert) {
            tiles.emplace_back(true, i, -1, types[i]);
        } else {
            tiles.emplace_back(false, i, *dice_it, types[i]);
            tilesDieIndex[*dice_it - 2].push_back(static_cast<int>(tiles.size()) - 1);
            dice_it++;
        }
    }

    points.reserve(Board::NUM_POINTS);
    for (Board::PointID id = 0; id < Board::NUM_POINTS; ++id) {
        points.emplace_back(id);
    }

    for (size_t tileID = 0; tileID < Board::NUM_TILES; ++tileID) {
        for (Board::PointID pointID : Board::tilePoints[tileID]) {
            for (int i = 0; i < Board::MAX_ADJACENT_TILES; ++i) {
                if (points[pointID].tiles[i] == Board::NON_TILE) {
                    points[pointID].tiles[i] = tileID;
                    break;
                }
            }
        }
    }

    edges.reserve(Board::NUM_EDGES);
   
    Board::EdgeID id = 0;

    std::set<std::pair<Board::PointID, Board::PointID>> exists;
    
    for (Board::TileID tileID = 0; tileID < Board::NUM_TILES; ++tileID) {
        for (int i = 0; i < static_cast<int>(Board::NUM_POINTS_PER_TILE); ++i) {

            Board::PointID a = Board::tilePoints[tileID][i];
            Board::PointID b = Board::tilePoints[tileID][(i + 1) % 6];

            if (a > b) {
                std::swap(a, b);
            }

            auto pair = std::make_pair(a, b);
            if (exists.find(pair) != exists.end()) continue;
            
            exists.insert(pair);

            edges.emplace_back(id, a, b);
            
            for (auto &edge : points[a].edges) {
                if (edge == Board::NON_EDGE) {
                    edge = id;
                    break;
                }
            }
            for (auto &edge : points[b].edges) {
                if (edge == Board::NON_EDGE) {
                    edge = id;
                    break;
                }
            }
            id++;
        }
    }

    for (const auto &point : points) {
        for (size_t i = 0; i < Board::MAX_ADJACENT_EDGES_TO_POINT && point.edges[i] != Board::NON_EDGE; ++i) {
            for (size_t j = 0; j < Board::MAX_ADJACENT_EDGES_TO_POINT && point.edges[j] != Board::NON_EDGE; ++j) {
                if (point.edges[i] == point.edges[j]) continue;
                size_t pos = 0;
                while (pos < Board::MAX_ADJACENT_EDGES_TO_EDGE && edges[point.edges[i]].edges[pos] != Board::NON_EDGE) {
                    ++pos;
                }
                if (pos >= Board::MAX_ADJACENT_EDGES_TO_EDGE) 
                    throw std::runtime_error("tried to insert edge in a pos higher than maximum number of adjacent edges to an edge.");

                edges[point.edges[i]].edges[pos] = point.edges[j];
            }
        }
    }

}

void GameBoard::printBoard() const {
    std::cout << "============================== Printing Board ==============================\n";
    std::cout << "============================== Printing Tiles ==============================\n";
    _printTiles();
    std::cout << "============================== Printing Points ==============================\n";
    _printPoints();
    std::cout << "============================== Printing Edges ==============================\n";
    _printEdges();
}

GameBoard::GameBoard() {
    createBoard();
}

} // end Catan namespace