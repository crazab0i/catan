#pragma once

#include "bank.hpp"

#include <cstdint>

namespace Catan {

using PlayerID = uint8_t;

class Player {
    private:
    PlayerID id;

    // sheep, wood, wheat, brick, ore. Each player starts with enough to build 2 settlements and 2 roads
    std::array<Economy::CardCount, Card::NUM_RESOURCE_TYPE> resourceInventory = {2, 4, 2, 4, 0};

    std::array<Economy::CardCount, Card::NUM_DEV_TYPE> developmentCardInventory = {0, 0, 0, 0, 0};

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

    Player(PlayerID id);

    bool canBuyBuildable(const Economy::Buildable type) const;

    const std::array<Economy::CardCount, Card::NUM_RESOURCE_TYPE>& getResourceInventory() const;
    const std::array<Economy::CardCount, Card::NUM_DEV_TYPE>& getDevelopmentCardInventory() const;

    void recieveResources(const std::array<Economy::CardCount, Card::NUM_RESOURCE_TYPE> &cardCounts);
    void recieveDevCard(const Card::Development card);

    void expendResources(const std::array<Economy::CardCount, Card::NUM_RESOURCE_TYPE> &cardCounts);
    void expendDevCard(const Card::Development card);
};


} // end Catan namespace