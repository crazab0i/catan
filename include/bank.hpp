#pragma once

#include <array>
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

constexpr const char* DevType_to_string(Development type) {
    switch (type) {
        case Development::Knight:       return "Knight";
        case Development::VictoryPoint: return "Victory Point";
        case Development::RoadBuilding: return "Road Building";
        case Development::YearOfPlenty: return "Year of Plenty";
        case Development::Monopoly:     return "Monopoly";
        default:                        return "";
    }
}

} // end Card namespace

class Bank {
    private:
        std::array<int, Card::NUM_RESOURCE_TYPE> resources;

        std::vector<Card::Development> developmentDeck;
        

        void _resetResources();
        void _resetDevelopmentDeck();

        void _printResources() const;
        void _printDevelopmentDeck() const;
    public:
        void printBank() const;

        void reset();


        bool deal_cards(std::vector<int> counts, Card::Resource type);
        bool trade(int count, Card::Resource inType, Card::Resource outType);
        Card::Development draw_developmentCard();

        Bank();
};
} // end Catan namespace