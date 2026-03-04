#pragma once

#include <array>
#include <deque>
#include <stdexcept>
#include <vector>

//////////////////////////////////////////////////////////////////////////
//
//      Catan Bank has 5 types of resource cards, 19 each
//          
//          There are 25 development cards,    
//              14 Knights
//              5 Victory Point
//              2 Road Building
//              2 Year of Plenty
//              2 Monopoly
//          
//
//////////////////////////////////////////////////////////////////////////

namespace Catan {

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

namespace Economy {

enum class Buildable {
    Road,
    Settlement,
    City,
    DevelopmentCard,
    _Count,
};

constexpr size_t NUM_BUILDABLES = static_cast<size_t>(Buildable::_Count);

using CardCount = int;

// cost of road, settlement, city, devCard
inline constexpr std::array<std::array<CardCount, Card::NUM_RESOURCE_TYPE>, NUM_BUILDABLES> buildCosts = {{
    //Sheep, Wood, Wheat, Brick, Ore
    {0, 1, 0, 1, 0},
    {1, 1, 1, 1, 0},
    {0, 0, 2, 0, 3},
    {1, 0, 1, 0, 1},
}};

} // end Economy namespace

class Bank {
    private:

        std::array<Economy::CardCount, Card::NUM_RESOURCE_TYPE> resourceCounts;

        std::deque<Card::Development> developmentDeck;
        std::deque<Card::Development> discardDeck;

        void _resetResources();
        void _resetDevelopmentDeck();

        void _printResources() const;
        void _printDevelopmentDeck() const;

        public:
        void printBank() const;

        void reset();

        Bank();

        const std::array<Economy::CardCount, Card::NUM_RESOURCE_TYPE>& getResourceCounts() const;

        bool dealCards(const std::vector<Economy::CardCount> &playerPayout, const Card::Resource type);
        void buyBuildable(const Economy::Buildable type);
        void trade(const Economy::CardCount inCount, const Economy::CardCount outCount, const Card::Resource inType, const Card::Resource outType);
        
        bool developmentCardAvailable();
        Card::Development drawDevelopmentCard();
        void discardDevelopmentCard();
};
} // end Catan namespace