
#pragma once

#include <array>
#include <vector>

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

inline constexpr std::array<std::array<EdgeID, NUM_EDGES_PER_TILE>, NUM_TILES> tileEdges = {{
    // Row 1
    {{ 0,  1,  7, 12, 11,  6}},
    {{ 2,  3,  8, 14, 13,  7}},
    {{ 4,  5,  9, 16, 15,  8}},

    // Row 2
    {{10, 11, 19, 25, 24, 18}},
    {{12, 13, 20, 27, 26, 19}},
    {{14, 15, 21, 29, 28, 20}},
    {{16, 17, 22, 31, 30, 21}},

    // Row 3
    {{23, 24, 34, 40, 39, 33}},
    {{25, 26, 35, 42, 41, 34}},
    {{27, 28, 36, 44, 43, 35}},
    {{29, 30, 37, 46, 45, 36}},
    {{31, 32, 38, 48, 47, 37}},

    // Row 4
    {{39, 40, 50, 55, 54, 49}},
    {{41, 42, 51, 57, 56, 50}},
    {{43, 44, 52, 59, 58, 51}},
    {{45, 46, 53, 61, 60, 52}},

    // Row 5
    {{54, 55, 63, 67, 66, 62}},
    {{56, 57, 64, 69, 68, 63}},
    {{58, 59, 65, 71, 70, 64}}
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

enum class Building {
    Road,
    Settlement,
    City,
};  

struct Point {
    PointID id;
    TileID tile_id;

    Point(int id, int tile_id);
};

struct Edge {
    EdgeID id;

    Edge(int id);
};

struct Tile {
    bool hasRobber;
    TileID id;
    int diceValue;
    TileType type;

    Tile(bool hasRobber, int id, int diceValue, TileType type);
    void printTile();
};

} // end Board namespace

class GameBoard {
    private:
        std::vector<Board::Tile> tiles;

    public:
        void createBoard();
        void printBoard();
        GameBoard();
};



} // end Catan namespace