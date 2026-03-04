#include "player.hpp"
#include "bank.hpp"

#include <array>
#include <iostream>
#include <stdexcept>

namespace Catan {

//////////////////////////////////////////////////////////////////////////
//
//      Player Class
//
/////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    //
    //      Private
    //
    /////////////////////////////////////////////////////////////////////////

void Player::_modifyResourceInventory(const std::array<Economy::CardCount, Card::NUM_RESOURCE_TYPE> &cardCounts, const Mode mode) {
    
    if (mode == Mode::Addition) {
        for (size_t i = 0; i < Card::NUM_RESOURCE_TYPE; ++i) {
            resourceInventory[i] += cardCounts[i];
        }
    } else {
        for (size_t i = 0; i < Card::NUM_RESOURCE_TYPE; ++i) {
            if (cardCounts[i] > resourceInventory[i]) throw std::runtime_error("tried to withdraw more cards than available");
            resourceInventory[i] -= cardCounts[i];
        }
    }
}

void Player::_modifyDevelopmentCardInventory(const Card::Development card, Mode mode) {
    
    if (mode == Mode::Addition) {
        developmentCardInventory[static_cast<size_t>(card)]++;
    } else {
        if (developmentCardInventory[static_cast<size_t>(card)] == 0) throw std::runtime_error("tried to remove a dev card that doesn't exist");
        developmentCardInventory[static_cast<size_t>(card)]--;
    }
}

void Player::_printResourceInventory() const {
    for (size_t i = 0; i < Card::NUM_RESOURCE_TYPE; ++i) {
        std::cout << Card::resourceTypeToString(static_cast<Card::Resource>(i)) << ": " 
            <<  resourceInventory[i] << "\n";
    }
}

void Player::_printDevelopmentCardInventory() const {
    for (size_t i = 0; i < Card::NUM_DEV_TYPE; ++i) {
        std::cout << Card::devTypeToString(static_cast<Card::Development>(i)) << ": "
            << developmentCardInventory[i] << "\n";
    }
}

    //////////////////////////////////////////////////////////////////////////
    //
    //      Public
    //
    /////////////////////////////////////////////////////////////////////////

void Player::printPlayer() const {
    std::cout << "============================== Printing Player: " << id << "==============================\n";
    std::cout << "============================== Printing Resource Inventory ==============================\n";
    _printResourceInventory();
    std::cout << "============================== Printing DevCard Inventory ==============================\n";
    _printDevelopmentCardInventory();
}

Player::Player(PlayerID id) : id(id) {};

bool Player::canBuyBuildable(const Economy::Buildable type) const {
    
    for (size_t i = 0; i < Card::NUM_RESOURCE_TYPE; ++i) {
        if (resourceInventory[i] < Economy::buildCosts[static_cast<size_t>(type)][i]) return false;
    }
    return true;
}

const std::array<Economy::CardCount, Card::NUM_RESOURCE_TYPE>& Player::getResourceInventory() const {
    return resourceInventory;
}

const std::array<Economy::CardCount, Card::NUM_DEV_TYPE>& Player::getDevelopmentCardInventory() const {
    return developmentCardInventory;
}

void Player::recieveResources(const std::array<Economy::CardCount, Card::NUM_RESOURCE_TYPE> &cardCounts) {
    _modifyResourceInventory(cardCounts, Mode::Addition);
}

void Player::recieveDevCard(const Card::Development card) {
    _modifyDevelopmentCardInventory(card, Mode::Addition);
}

void Player::expendResources(const std::array<Economy::CardCount, Card::NUM_RESOURCE_TYPE> &cardCounts) {
    _modifyResourceInventory(cardCounts, Mode::Deletion);
}

void Player::expendDevCard(const Card::Development card) {
    _modifyDevelopmentCardInventory(card, Mode::Deletion);
}


} // end Catan namespace