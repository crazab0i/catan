#pragma once

#include <cstddef>
#include <stdexcept>
#include <array>
#include <cstdint>
#include <limits>
#include <optional>

namespace Catan {

//////////////////////////////////////////////////////////////////////////
//
//      GameDefs Namespace
//
/////////////////////////////////////////////////////////////////////////

namespace GameDefs {

constexpr int NUM_MAX_PLAYERS = 4;

using PlayerID = uint8_t;
using DieVal = uint8_t;

constexpr DieVal ROBBER_ROLL = 7;

enum class Stage {
    Start,
    Setup,
    Main,
};

enum class Turn {
    Player0,
    Player1,
    Player2,
    Player3,
    End,
};

enum class TurnStage {
    Roll,
    MoveRobber,
    RoadBuilding,
    Build,
    None,
};

enum class SetupStage {
    RollForFirstPlayer,
    FirstSettlementPlacement,
    SecondSettlementPlacement,
    Complete,
};

struct SetupState {
    SetupStage stage;

    DieVal highestRoll = 0;
    PlayerID highestRollPlayer = 0;

    int placementRoundNum = 1;
    PlayerID firstPlayer;
    PlayerID lastPlayer;
};

constexpr const char *stageToString(Stage stage) {
    switch (stage) {
        case Stage::Start:  return "Start";
        case Stage::Setup:  return "Setup";
        case Stage::Main:   return "Main";
        default:    throw std::runtime_error("unreachable");
    }
}

constexpr const char *turnStageToString(TurnStage stage) {
    switch (stage) {
        case TurnStage::Roll:           return "Roll";
        case TurnStage::MoveRobber:     return "MoveRobber";
        case TurnStage::RoadBuilding:   return "RoadBuilding";
        case TurnStage::Build:          return "Build";
        case TurnStage::None:           return "None";
        default:    throw std::runtime_error("unreachable");
    }
}

constexpr const char *setupStateToString(SetupStage stage) {
    switch (stage) {
        case SetupStage::Complete:                  return "Complete";
        case SetupStage::FirstSettlementPlacement:  return "FirstSettlementPlacement";
        case SetupStage::SecondSettlementPlacement: return "SecondSettlementPlacement";
        case SetupStage::RollForFirstPlayer:        return "RollForFirstPlayer";
        default:    throw std::runtime_error("unreachable");
    }
}

} // end GameDefs namespace


//////////////////////////////////////////////////////////////////////////
//
//      Card Namespace
//
/////////////////////////////////////////////////////////////////////////

namespace Card {

enum class Resource {
    Sheep, 
    Wood,
    Wheat,
    Brick,
    Ore,
    _Count, 
};

constexpr int         NUM_RESOURCE = 19;
constexpr size_t NUM_RESOURCE_TYPE = static_cast<size_t>(Resource::_Count);

constexpr const char* resourceTypeToString(Resource type) {
    switch (type) {
        case Resource::Sheep:   return "Sheep";
        case Resource::Wood:    return "Wood";
        case Resource::Wheat:   return "Wheat";
        case Resource::Brick:   return "Brick";
        case Resource::Ore:     return "Ore";
        default:    throw std::runtime_error("tried to convert invalid resource to string");
    }
}

enum class Development {
    Knight,
    VictoryPoint,
    RoadBuilding,
    YearOfPlenty,
    Monopoly,
    _Count,
};

constexpr int   NUM_KNIGHTS = 14;
constexpr int   NUM_VICTORY_POINTS = 5;
constexpr int   NUM_ROAD_BUILDING = 2;
constexpr int   NUM_YEAR_PLENTY = 2;
constexpr int   NUM_MONOPOLY = 2;
constexpr size_t NUM_DEV_TYPE = static_cast<size_t>(Development::_Count);
constexpr int   NUM_DEV_CARDS = 25;

inline constexpr std::array<std::pair<Development, int>, NUM_DEV_TYPE> developmentCounts = {{
    {Development::Knight,       NUM_KNIGHTS},
    {Development::VictoryPoint, NUM_VICTORY_POINTS},
    {Development::RoadBuilding, NUM_ROAD_BUILDING},
    {Development::YearOfPlenty, NUM_YEAR_PLENTY},
    {Development::Monopoly,     NUM_MONOPOLY},

}};

constexpr const char* devTypeToString(Development type) {
    switch (type) {
        case Development::Knight:       return "Knight";
        case Development::VictoryPoint: return "Victory Point";
        case Development::RoadBuilding: return "Road Building";
        case Development::YearOfPlenty: return "Year of Plenty";
        case Development::Monopoly:     return "Monopoly";
        default:    throw std::runtime_error("tried to convert invalid dev type to string");
    }
}
} // end Card namespace


//////////////////////////////////////////////////////////////////////////
//
//      Economy Namespace
//
/////////////////////////////////////////////////////////////////////////

namespace Economy {

enum class TradeTarget {
    Player,
    Bank,
};

enum class Buildable {
    Road,
    Settlement,
    City,
    DevelopmentCard,
    _Count,
};

constexpr size_t NUM_BUILDABLES = static_cast<size_t>(Buildable::_Count);

using CardCount = int;
using ResourceArray = std::array<Economy::CardCount, Card::NUM_RESOURCE_TYPE>;
using PlayerPayout = std::array<Economy::CardCount, GameDefs::NUM_MAX_PLAYERS>;

// cost of road, settlement, city, devCard
inline constexpr std::array<std::array<CardCount, Card::NUM_RESOURCE_TYPE>, NUM_BUILDABLES> buildCosts = {{
    //Sheep, Wood, Wheat, Brick, Ore
    {0, 1, 0, 1, 0},
    {1, 1, 1, 1, 0},
    {0, 0, 2, 0, 3},
    {1, 0, 1, 0, 1},
}};

} // end Economy namespace


//////////////////////////////////////////////////////////////////////////
//
//      Board Namespace
//
/////////////////////////////////////////////////////////////////////////

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

constexpr GameDefs::PlayerID NO_OWNER = UINT8_MAX;
constexpr int INITIAL_CARD_COUNT = 0;
constexpr int SETTLEMENT_RESOURCE_PAYOUT = 1;
constexpr int CITY_RESOURCE_PAYOUT = 2;

struct Building {
    GameDefs::PlayerID ownerID = NO_OWNER;
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


//////////////////////////////////////////////////////////////////////////
//
//      API Namespace
//
/////////////////////////////////////////////////////////////////////////

namespace API {

/**
 * Represents the result of a dice roll.
 * Contains the total and individual die values.
 */
struct FirstRollResult {
    std::pair<GameDefs::DieVal, GameDefs::DieVal> diceRoll;
    
    GameDefs::DieVal dieTotal;
    GameDefs::PlayerID playerRolled;

    GameDefs::DieVal highestRolled;
    GameDefs::PlayerID highestPlayerRolled;
};

struct RollResult {
    std::pair<GameDefs::DieVal, GameDefs::DieVal> diceRoll;
    GameDefs::DieVal dieTotal;

    std::array<std::optional<Economy::PlayerPayout>, Card::NUM_RESOURCE_TYPE> payouts;

    GameDefs::PlayerID player;
};

struct DevResult {
    Card::Development card;
    // special cases robber, monopoly, road building, invention
};


} // end API Namespace

} // end Catan namespace