#include "board.hpp"
#include "catanConsts.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <format>
#include <iostream>
#include <iterator>
#include <optional>
#include <random>
#include <set>
#include <stdexcept>
#include <sys/types.h>
#include <unordered_map>
#include <unordered_set>
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
        default:                        throw std::runtime_error("unreachable");
    }
    std::cout << "\n";
}   


Point::Point(uint8_t id) : id(id), building({BuildLocationTypes::Point, id}) {}
Edge::Edge(uint8_t id, PointID a, PointID b) : id(static_cast<EdgeID>(id)), a(a), b(b), building({BuildLocationTypes::Edge, id}) {}
Tile::Tile(bool hasRobber, 
           uint8_t id, 
           GameDefs::DieVal diceValue,
           TileType type) : hasRobber(hasRobber), id(id), diceValue(diceValue), type(type) {}

void Tile::printTile() const {
    std::cout << "========== tile id: " << static_cast<int>(id) << " ==========\n";
    std::cout << "type: " << tileTypeToString(type);
    std::cout << "\ndice value: " << static_cast<int>(diceValue);
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
            if (adjacent_edge == Board::NON_EDGE) {
                std::cout << "NON_EDGE ";
            } else {
                std::cout << static_cast<int>(adjacent_edge) << " ";
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

void GameBoard::_printPorts() const {
    for (size_t i = 0; i < pointPorts.size(); ++i) {
        std::cout << std::format("port {} has ", static_cast<int>(i));
        if (pointPorts[i]) {
            std::cout << std::format("port {}", Board::portTypeToString(pointPorts[i].value()));
        } else {
            std::cout << "no port";
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

void GameBoard::_createTiles() {
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
}

void GameBoard::_createPointsAndEdges() {
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
            for (size_t j = i + 1; j < Board::MAX_ADJACENT_EDGES_TO_POINT && point.edges[j] != Board::NON_EDGE; ++j) {

                auto &edge1 = edges[point.edges[i]];
                auto &edge2 = edges[point.edges[j]];

                size_t pos = 0;
                while (pos < Board::MAX_ADJACENT_EDGES_TO_EDGE && edge1.edges[pos] != Board::NON_EDGE)
                    ++pos;

                if (pos >= Board::MAX_ADJACENT_EDGES_TO_EDGE) 
                    throw std::runtime_error("tried to insert edge in a pos higher than maximum number of adjacent edges to an edge.");

                edge1.edges[pos] = edge2.id;

                pos = 0;
                while (pos < Board::MAX_ADJACENT_EDGES_TO_EDGE && edge2.edges[pos] != Board::NON_EDGE)
                    ++pos;
                
                if (pos >= Board::MAX_ADJACENT_EDGES_TO_EDGE) 
                    throw std::runtime_error("tried to insert edge in a pos higher than maximum number of adjacent edges to an edge.");

                edge2.edges[pos] = edge1.id;

            }
        }
    }
}

void GameBoard::_createValidPointsAndEdges() {
    for (Board::PointID id = 0; id < Board::NUM_POINTS; ++id) {
        validPoints.insert(id);
    }

    for (Board::EdgeID id = 0; id < Board::NUM_EDGES; ++id) {
        validEdges.insert(id);
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

const std::set<Board::PointID> GameBoard::_getAdjacentPoints(const Board::PointID pointID) const {

    std::set<Board::PointID> result;
    const auto point = points[pointID];
    
    for (const auto edgeID : point.edges) {
        const auto edge = edges[edgeID];
        const auto otherPoint = (edge.a == pointID) ? edge.b : edge.a;
        result.insert(otherPoint);
    }
    return result;
}

void GameBoard::_invalidatePointAndAdjacentPoints(const Board::PointID pointID) {
    
    auto it = validPoints.find(pointID);
    if (it != validPoints.end()) validPoints.erase(it);

    const auto adjacentPoints = _getAdjacentPoints(pointID);

    for (const auto point : adjacentPoints) {
        it = validPoints.find(point);
        if (it != validPoints.end()) validPoints.erase(it);
    }
}

void GameBoard::_checkBuildingLocationIsValid(const Board::Building &building, const Board::BuildPhase phase) const {

    if (building.buildingType == Board::BuildingType::Road) {
        const auto &playerValidEdges = validPlayerEdges[static_cast<size_t>(building.ownerID)];
        std::unordered_set<Board::EdgeID> validEdgesIntersection;

        for (const auto edge : playerValidEdges) {
            if (validEdges.contains(edge)) validEdgesIntersection.insert(edge);
        }
        if (!validEdgesIntersection.contains(building.position.value))
            throw std::runtime_error(std::format("expected road to be on valid point, instead recieved invalid pointID: {}", static_cast<int>(building.position.value)));

    } else if (building.buildingType == Board::BuildingType::Settlement && phase == Board::BuildPhase::Setup) {
        if (!validEdges.contains(building.position.value))
            throw std::runtime_error(std::format("expected settlement to be on valid point, instead recieved invalid pointID: {}", static_cast<int>(building.position.value)));
    } else if (building.buildingType == Board::BuildingType::Settlement && phase == Board::BuildPhase::Main) {
        
    } else if (building.buildingType == Board::BuildingType::City) {
        const auto &settlement = points[building.position.value].building;
        if (settlement.buildingType != Board::BuildingType::Settlement) 
            throw std::runtime_error(std::format("expected city to be built on settlement, instead recieved pointID: {}, with building type: {}", static_cast<int>(building.position.value), Board::buildingTypeToString(building.buildingType)));
        if (settlement.ownerID != building.ownerID)
            throw std::runtime_error(std::format("expected city to be built on owned settlement, instead found settlement owned by: {}, builder is: {}", static_cast<int>(settlement.ownerID), static_cast<int>(building.ownerID)));
    }
}

void GameBoard::_validateAdjacent(const Board::Building &building) {
    switch (building.buildingType) {
        case Board::BuildingType::Road: {
            auto &playerValidEdges = validPlayerEdges[static_cast<size_t>(building.ownerID)];
            const auto &edge = edges[building.position.value];
            const auto &adjacentEdges = edge.edges;
            
            for (const auto edge : adjacentEdges) {
                if (edge == Board::NON_EDGE) break;

                playerValidEdges.insert(edge);
            }

            auto &playerValidPoints = validPlayerPoints[static_cast<size_t>(building.ownerID)];
            playerValidPoints.insert(edge.a);
            playerValidPoints.insert(edge.b);
            break;
        }
        case Board::BuildingType::Settlement: {
            auto &playerValidEdges = validPlayerEdges[static_cast<size_t>(building.ownerID)];
            const auto &adjacentEdges = points[building.position.value].edges;

            for (const auto edge : adjacentEdges) {
                if (edge == Board::NON_EDGE) break;

                playerValidEdges.insert(edge);
            }
            break;
        }
        default: break;
    }
}

    //////////////////////////////////////////////////////////////////////////
    //
    //      Public
    //
    /////////////////////////////////////////////////////////////////////////

void GameBoard::createBoard() {
    
    _createTiles();

    _createPointsAndEdges();
    
    _createValidPointsAndEdges();

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

const std::set<Board::PointID>& GameBoard::getValidPoints() const {
    return validPoints;
}

const std::set<Board::EdgeID>& GameBoard::getValidEdges() const {
    return validEdges;
}

std::optional<Board::PortType> GameBoard::placeBuilding(const Board::Building &building, const Board::BuildPhase phase) {

    _checkBuildingLocationIsValid(building, phase);
    _validateAdjacent(building);

    if (building.buildingType == Board::BuildingType::Settlement) {
        points[building.position.value].building = building;

        _invalidatePointAndAdjacentPoints(building.position.value);

        if (pointPorts[building.position.value]) return pointPorts[building.position.value];

    } else if (building.buildingType == Board::BuildingType::City) {
        points[building.position.value].building = building;

    } else if (building.buildingType == Board::BuildingType::Road) {
        edges[building.position.value].building = building;
        
        validEdges.erase(validEdges.find(building.position.value));
    }

    return std::nullopt;
}

const std::vector<Board::TileID> GameBoard::getValidRobberDestinations() const {
    
    std::vector<Board::TileID> valid(Board::NUM_TILES);
    std::iota(begin(valid), end(valid), Board::TileID{0});
    valid.erase(std::remove(begin(valid), end(valid), currentRobberTile), end(valid));
    
    return valid;
}

std::set<GameDefs::PlayerID> GameBoard::placeRobber(const Board::TileID destination) {
    
    tiles[currentRobberTile].hasRobber = false;
    tiles[destination].hasRobber = true;

    std::set<GameDefs::PlayerID> robbedPlayers;

    for (const auto point : Board::tilePoints[destination]) {
        if (points[point].building.ownerID == Board::NO_OWNER) continue;
        robbedPlayers.insert(points[point].building.ownerID);
    }

    return robbedPlayers;
}

const std::array<Economy::PlayerPayout, Card::NUM_RESOURCE_TYPE> GameBoard::getRollPayout(const GameDefs::DieVal dieVal) const {

    std::array<Economy::PlayerPayout, Card::NUM_RESOURCE_TYPE> payout;

    for (const auto tileID : tilesDieIndex[dieVal]) {
        
        const auto resource = _mapTileToCard(tiles[tileID].type);
        auto &playerPayout = payout[static_cast<int>(resource)];

        for (const auto pointID : Board::tilePoints[tileID]) {
            const auto building = points[pointID].building;
            if (building.ownerID == Board::NO_OWNER) continue;

            if (building.buildingType == Board::BuildingType::Settlement) {
                playerPayout[building.ownerID] += Board::SETTLEMENT_RESOURCE_PAYOUT;
            } else {
                playerPayout[building.ownerID] += Board::CITY_RESOURCE_PAYOUT;
            }
        }
    }

    return payout;
}

const Economy::ResourceArray GameBoard::getInitialSettlementPayout(const Board::PointID secondPlacement) const {
    
    Economy::ResourceArray payout = {0, 0, 0, 0, 0,};

    constexpr int NUM_MAX_TILES_PER_POINT = 3;
    int found = 0;

    for (size_t i = 0; i < Board::NUM_TILES; ++i) {
        for (const auto pointID : Board::tilePoints[i]) {
            if (pointID == secondPlacement) {
                ++payout[static_cast<int>(tiles[i].type)];
                ++found;
                break;
            }
        }
        if (found == NUM_MAX_TILES_PER_POINT) {
            break;
        }
    }

    return payout;
}

Board::TileID GameBoard::getCurrentRobberPosition() const {
    return currentRobberTile;
}

} // end Catan namespace