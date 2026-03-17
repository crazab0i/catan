#pragma once

#include "catanConsts.hpp"

#include <array>

namespace Catan {

class Player {
    private:
    GameDefs::PlayerID id;
    int victoryPoints = 0;

    int knightCount = 0;
    
    Economy::ResourceArray resourceInventory = {0, 0, 0, 0, 0};

    std::array<Economy::CardCount, Card::NUM_DEV_TYPE> developmentCardInventory = {0, 0, 0, 0, 0};
    std::array<Board::BuildingCount, Board::NUM_BUILDINGS> buildingInventory = Board::startingBuildingCounts; 

    enum class Mode {
        Addition,
        Deletion,
    };

    void _modifyResourceInventory(const std::array<Economy::CardCount, Card::NUM_RESOURCE_TYPE> &cardCounts, const Mode mode);
    void _modifyDevelopmentCardInventory(const Card::Development card, Mode mode);

    void _printResourceInventory() const;
    void _printDevelopmentCardInventory() const;

    public:

    void printPlayer() const;

    Player(GameDefs::PlayerID id);

    bool canBuyBuildable(const Economy::Buildable type) const;

    const std::array<Economy::CardCount, Card::NUM_RESOURCE_TYPE>& getResourceInventory() const;
    const std::array<Economy::CardCount, Card::NUM_DEV_TYPE>& getDevelopmentCardInventory() const;

    void recieveResources(const std::array<Economy::CardCount, Card::NUM_RESOURCE_TYPE> &cardCounts);
    void recieveDevCard(const Card::Development card);

    void expendResources(const std::array<Economy::CardCount, Card::NUM_RESOURCE_TYPE> &cardCounts);
    void expendDevCard(const Card::Development card);
};


} // end Catan namespace