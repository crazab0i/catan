#include "board.hpp"
#include "bank.hpp"
#include "player.hpp"
#include "game.hpp"

#include <cstddef>
#include <iostream>
#include <algorithm>
#include <random>
#include <set>
#include <stdexcept>
#include <unordered_map>
#include <utility>

namespace Catan {

namespace Board {


//////////////////////////////////////////////////////////////////////////
//
//      STRUCTS
//
/////////////////////////////////////////////////////////////////////////

void Building::printBuilding() {
    std::cout << "owner_id: " << ownerID << "\n";
    std::cout << "building_type: ";
    switch (buildingType) {
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

    //////////////////////////////////////////////////////////////////////////
    //
    //      Private
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

void GameBoard::_printTileDieIndex() const {
    for (size_t i = 0; i < tilesDieIndex.size(); ++i) {
        std::cout << "die value: " << (i + 2) << "\n";
        for (const int tileID : tilesDieIndex[i]) {
            std::cout << tileID << " ";
        }
        std::cout << "\n";
    }
}

Card::Resource GameBoard::_mapTileToCard(Board::TileType type) const {
    
    switch (type) {
        case Board::TileType::Sheep:    return Card::Resource::Sheep;
        case Board::TileType::Wood:     return Card::Resource::Wood;
        case Board::TileType::Wheat:    return Card::Resource::Wheat;
        case Board::TileType::Brick:    return Card::Resource::Brick;
        case Board::TileType::Ore:      return Card::Resource::Ore;

        default: throw std::runtime_error("recieved a type incompitable with card");
    }
}

void GameBoard::_createPortPoints() {
    constexpr int NUM_GENERAL_PORTS = 8;
    constexpr int NUM_SPECIAL_PORTS = 2;
    constexpr size_t NUM_SPECIAL_PORT_TYPES = static_cast<size_t>(Board::PortType::_Count);

    std::array<Board::PointID, NUM_GENERAL_PORTS> generalPointIDs = {0, 1, 26, 37, 47, 48, 50, 51};
    std::array<std::array<Board::PointID, NUM_SPECIAL_PORTS>, NUM_SPECIAL_PORT_TYPES> specialPointIDs = {{
        {45, 46},
        {7, 17},
        {3, 4},
        {28, 38},
        {14, 15},
    }};

    std::array<Board::PortType, NUM_SPECIAL_PORT_TYPES> specialPorts = {
        Board::PortType::Sheep, 
        Board::PortType::Wood, 
        Board::PortType::Wheat, 
        Board::PortType::Brick, 
        Board::PortType::Ore
    };

    for (const auto pointID : generalPointIDs) {
        pointPorts[pointID] = Board::PortType::General;
    }

    for (size_t i = 0; i < NUM_SPECIAL_PORT_TYPES; ++i) {
        for (const auto pointID : specialPointIDs[i]) {
            pointPorts[pointID] = specialPorts[i];
        }
    }
}

    //////////////////////////////////////////////////////////////////////////
    //
    //      Public
    //
    /////////////////////////////////////////////////////////////////////////

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
            currentRobberTile = static_cast<Board::TileID>(i);
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

        _createPortPoints();
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

    for (Board::PointID id = 0; id < Board::NUM_POINTS; ++id) {
        validPoints.insert(id);
    }

    for (Board::EdgeID id = 0; id < Board::NUM_EDGES; ++id) {
        validEdges.insert(id);
    }

    _createPortPoints();
}

void GameBoard::printBoard() const {
    std::cout << "============================== Printing Board ==============================\n";
    std::cout << "============================== Printing Tiles ==============================\n";
    _printTiles();
    std::cout << "============================== Printing Points ==============================\n";
    _printPoints();
    std::cout << "============================== Printing Edges ==============================\n";
    _printEdges();
    std::cout << "============================== Printing TileDieIndex ==============================\n";
    _printTileDieIndex();
}

GameBoard::GameBoard() {
    createBoard();
}

void GameBoard::placeBuilding(Board::Building &&building, const BuildDestination &destination) {
    
    std::visit([&](auto &&arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, Board::PointID>) {

            if (building.buildingType != Board::BuildingType::Settlement && building.buildingType != Board::BuildingType::City)
                throw std::runtime_error("tried to build a non-point builing on a point");
            if (building.buildingType == Board::BuildingType::Settlement && points[arg].building.buildingType != Board::BuildingType::Unbuilt)
                throw std::runtime_error("tried to build settlement on a non-empty point");
            if (building.buildingType == Board::BuildingType::City && points[arg].building.buildingType != Board::BuildingType::City)
                throw std::runtime_error("tried to build city on a point without a settlement");

            points[arg].building = std::move(building);

            auto it = validPoints.find(arg);
            if (it != validPoints.end()) validPoints.erase(it);

        } else {

            if (building.buildingType != Board::BuildingType::Road)
                throw std::runtime_error("tried to build a non-road on edge");
            if (edges[arg].building.buildingType != Board::BuildingType::Unbuilt) {
                throw std::runtime_error("tried to build a road on an occupied edge");
            }

            edges[arg].building = std::move(building);

            auto it = validEdges.find(arg);
            if (it != validEdges.end()) validEdges.erase(it);
        }
    }, destination);
}

std::vector<Board::TileID> GameBoard::getValidRobberDestinations() const {
    
    std::vector<Board::TileID> valid(Board::NUM_TILES);
    std::iota(begin(valid), end(valid), Board::TileID{0});
    valid.erase(std::remove(begin(valid), end(valid), currentRobberTile), end(valid));
    
    return valid;
}

std::unordered_set<PlayerID> GameBoard::placeRobber(const Board::TileID destination) {
    
    tiles[currentRobberTile].hasRobber = false;
    tiles[destination].hasRobber = true;

    std::unordered_set<PlayerID> robbedPlayers;

    for (const auto point : Board::tilePoints[destination]) {
        if (points[point].building.ownerID == Board::NO_OWNER) continue;
        robbedPlayers.insert(points[point].building.ownerID);
    }

    return robbedPlayers;
}

std::unordered_map<Card::Resource, std::vector<int>> GameBoard::getRollPayout(const int dieVal) const {

    std::unordered_map<Card::Resource, std::vector<int>> payout;

    for (const auto tileID : tilesDieIndex[dieVal]) {

        if (tiles[tileID].hasRobber) continue;

        const auto resource = _mapTileToCard(tiles[tileID].type);
        auto &cardCounts = payout[resource];

        cardCounts.resize(PLAYER_COUNT, Board::INITIAL_CARD_COUNT);

        for (const auto pointID : Board::tilePoints[tileID]) {
            if (points[pointID].building.ownerID == Board::NO_OWNER) continue;
            
            if (points[pointID].building.buildingType == Board::BuildingType::Settlement) {
                cardCounts[points[pointID].building.ownerID] += Board::CITY_RESOURCE_PAYOUT;
            } else {
                cardCounts[points[pointID].building.ownerID] += Board::SETTLEMENT_RESOURCE_PAYOUT;
            }
        }
    }
    return payout;
}


} // end Catan namespace