
#pragma once

#include "bank.hpp"
#include "player.hpp"

#include <array>
#include <cstdint>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include <optional>

//////////////////////////////////////////////////////////////////////////
//
//      Catan board has 19 total hexes, oriented 3,4,5,4,3
//          There are 8 ports
//          
//      Hex Counts:
//          4 Sheep
//          4 Wood
//          4 wheat
//          3 Brick
//          3 Ore
//          1 desert
//
//
//////////////////////////////////////////////////////////////////////////



namespace Catan {

namespace Board {

using TileID = uint8_t;
using PointID = uint8_t;
using EdgeID = uint8_t;

constexpr size_t NUM_POINTS_PER_TILE = 6;
constexpr size_t NUM_EDGES_PER_TILE = 6;
constexpr size_t NUM_TILES = 19;

constexpr size_t NUM_POINTS = 54;
constexpr size_t NUM_EDGES = 72;

inline constexpr std::array<std::array<PointID, NUM_POINTS_PER_TILE>, NUM_TILES> tilePoints = {{
    // Row 1
    {{0, 1, 2, 10, 9, 8}},
    {{2, 3, 4, 12, 11, 10}},
    {{4, 5, 6, 14, 13, 12}},
    // Row 2
    {{7, 8, 9, 19, 18, 17}},
    {{9, 10, 11, 21, 20, 19}},
    {{11, 12, 13, 23, 22, 21}},
    {{13, 14, 15, 25, 24, 23}},
    // Row 3
    {{16, 17, 18, 29, 28, 27}},
    {{18, 19, 20, 31, 30, 29}},
    {{20, 21, 22, 33, 32, 31}},
    {{22, 23, 24, 35, 34, 33}},
    {{24, 25, 26, 37, 36, 35}},
    // Row 4
    {{28, 29, 30, 40, 39, 38}},
    {{30, 31, 32, 42, 41, 40}},
    {{32, 33, 34, 44, 43, 42}},
    {{34, 35, 36, 46, 45, 44}},
    // Row 5
    {{39, 40, 41, 49, 48, 47}},
    {{41, 42, 43, 51, 50, 49}},
    {{43, 44, 45, 53, 52, 51}}
}};

enum class TileType {
    Sheep,
    Wood,
    Wheat,
    Brick,
    Ore,
    Desert,
    _Count,          
};

constexpr int   NUM_SHEEP_TILE = 4;
constexpr int    NUM_WOOD_TILE = 4;
constexpr int   NUM_WHEAT_TILE = 4;
constexpr int   NUM_BRICK_TILE = 3;
constexpr int     NUM_ORE_TILE = 3;
constexpr int  NUM_DESERT_TILE = 1;
constexpr size_t NUM_TYPE_TILE = static_cast<size_t>(TileType::_Count);

    inline constexpr std::array<std::pair<TileType, int>, NUM_TYPE_TILE> tileCounts = {{
        {TileType::Sheep,   NUM_SHEEP_TILE},
        {TileType::Wood,    NUM_WOOD_TILE},
        {TileType::Wheat,   NUM_WHEAT_TILE},
        {TileType::Brick,   NUM_BRICK_TILE},
        {TileType::Ore,     NUM_ORE_TILE},
        {TileType::Desert,  NUM_DESERT_TILE},
    }};

constexpr const char* TileType_to_string(TileType type) {
    switch (type) {
        case TileType::Sheep:   return "Sheep";
        case TileType::Wood:    return "Wood";
        case TileType::Wheat:   return "Wheat";
        case TileType::Brick:   return "Brick";
        case TileType::Ore:     return "Ore";
        case TileType::Desert:  return "Desert";
        default:                return "";
    }
}

enum class PortType {
    General,
    Sheep,
    Wood,
    Wheat,
    Brick,
    Ore,
    _Count,
};

enum class BuildingType {
    Road,
    Settlement,
    City,
    Unbuilt,
};  

constexpr PlayerID NO_OWNER = -1;
constexpr int INITIAL_CARD_COUNT = 0;
constexpr int SETTLEMENT_RESOURCE_PAYOUT = 1;
constexpr int CITY_RESOURCE_PAYOUT = 2;

struct Building {
    PlayerID ownerID = NO_OWNER;
    BuildingType buildingType = BuildingType::Unbuilt;

    void printBuilding();
};

constexpr int MAX_ADJACENT_TILES = 3;
constexpr TileID NON_TILE = std::numeric_limits<TileID>::max();
constexpr int MAX_ADJACENT_EDGES_TO_POINT = 3;

constexpr EdgeID NON_EDGE = std::numeric_limits<EdgeID>::max();

struct Point {
    PointID id;
    std::array<TileID, MAX_ADJACENT_TILES> tiles {NON_TILE, NON_TILE, NON_TILE};
    std::array<EdgeID, MAX_ADJACENT_EDGES_TO_POINT>  edges {NON_EDGE, NON_EDGE, NON_EDGE};
    Building building;

    Point(int id);
};

constexpr int MAX_ADJACENT_EDGES_TO_EDGE = 4;

struct Edge {
    EdgeID id;
    
    PointID a;
    PointID b;

    std::array<EdgeID, MAX_ADJACENT_EDGES_TO_EDGE> edges {NON_EDGE, NON_EDGE, NON_EDGE, NON_EDGE};
    Building building;

    Edge(int id, PointID a, PointID b);
};

struct Tile {
    bool hasRobber;
    TileID id;
    int diceValue;
    TileType type;

    Tile(bool hasRobber, int id, int diceValue, TileType type);
    void printTile() const;
};

} // end Board namespace

class GameBoard {
    private:
        Board::TileID currentRobberTile = Board::NON_TILE;

        std::vector<Board::Tile> tiles;
        std::vector<std::vector<Board::TileID>> tilesDieIndex;

        std::vector<Board::Point> points;
        std::unordered_set<Board::PointID> validPoints;

        std::vector<Board::Edge> edges;
        std::unordered_set<Board::EdgeID> validEdges;

        std::array<std::optional<Board::PortType>, Board::NUM_POINTS> pointPorts;

        void _printTiles() const;
        void _printPoints() const;
        void _printEdges() const;
        void _printTileDieIndex() const;

        Card::Resource _mapTileToCard(Board::TileType type) const;

        void _createTiles();
        void _createPointsAndEdges();
        void _createValidPointsAndEdges();
        void _createPortPoints();

    public:
        void createBoard();
        void printBoard() const;
        
        GameBoard();

        const std::unordered_set<Board::PointID>& getValidPoints() const;
        const std::unordered_set<Board::EdgeID>& getValidEdges() const;

        using BuildDestination = std::variant<Board::TileID, Board::EdgeID>;
        std::optional<Board::PortType> placeBuilding(Board::Building &&building, const BuildDestination &destination);

        const std::vector<Board::TileID> getValidRobberDestinations() const;
        std::unordered_set<PlayerID> placeRobber(Board::TileID destination);

        std::unordered_map<Card::Resource, std::vector<int>> getRollPayout(const int dieVal) const;
};



} // end Catan namespace