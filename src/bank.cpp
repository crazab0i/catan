#include "bank.hpp"
#include "catanConsts.hpp"

#include <cstddef>
#include <numeric>
#include <random>
#include <iostream>
#include <algorithm>
#include <stdexcept>

namespace Catan {

//////////////////////////////////////////////////////////////////////////
//
//      Bank Class
//
/////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    //
    //      Private
    //
    /////////////////////////////////////////////////////////////////////////

void Bank::_resetResources() {
    for (size_t i = 0; i < Card::NUM_RESOURCE_TYPE; ++i) {
        resourceCounts[i] = Card::NUM_RESOURCE;
    }
}

void Bank::_resetDevelopmentDeck() {
    developmentDeck.clear();

    for (auto [type, count] : Card::developmentCounts) {
        developmentDeck.insert(end(developmentDeck), count, type);
    }    

    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(begin(developmentDeck), end(developmentDeck), gen);
}

void Bank::_printResources() const {
    std::cout << "NUM_SHEEP: "  << resourceCounts[static_cast<size_t>(Card::Resource::Sheep)] << "\n";
    std::cout << "NUM_WOOD: "   << resourceCounts[static_cast<size_t>(Card::Resource::Wood)] << "\n";
    std::cout << "NUM_WHEAT: "  << resourceCounts[static_cast<size_t>(Card::Resource::Wheat)] << "\n";
    std::cout << "NUM_BRICK: "  << resourceCounts[static_cast<size_t>(Card::Resource::Brick)] << "\n";
    std::cout << "NUM_ORE: "    << resourceCounts[static_cast<size_t>(Card::Resource::Ore)] << "\n";
}

void Bank::_printDevelopmentDeck() const {
    for (int i = Card::NUM_DEV_CARDS - 1; i >= 0; --i) {
        std::cout << Card::devTypeToString(developmentDeck[i]) << "\n";
    }
}

    //////////////////////////////////////////////////////////////////////////
    //
    //      Public
    //
    /////////////////////////////////////////////////////////////////////////

void Bank::printBank() const {
    std::cout << "==================================== Printing Bank ====================================\n";
    std::cout << "==================================== Printing Resources Deck ====================================\n";
    _printResources();
    std::cout << "==================================== Printing Development Card Deck ==============================\n";
    _printDevelopmentDeck();
}

void Bank::reset() {
    _resetResources();
    _resetDevelopmentDeck();
}

Bank::Bank() {
    reset();
}

const Economy::ResourceArray& Bank::getResourceCounts() const {
    return resourceCounts;
}

bool Bank::dealCards(const Economy::PlayerPayout &playerPayout, const Card::Resource type) {
    auto total = std::accumulate(begin(playerPayout), end(playerPayout), 0);
    if (total > resourceCounts[static_cast<size_t>(type)]) return false;

    resourceCounts[static_cast<size_t>(type)] -= total;
    return true;
}

void Bank::buyBuildable(const Economy::Buildable type) {
    
    for (size_t i = 0; i < Card::NUM_RESOURCE_TYPE; ++i) {
        resourceCounts[i] += Economy::buildCosts[static_cast<size_t>(type)][i];
    }
}

void Bank::trade(const Economy::CardCount inCount, const Economy::CardCount outCount, const Card::Resource inType, const Card::Resource outType) {
    if (outCount > resourceCounts[static_cast<size_t>(outType)]) throw std::runtime_error("tried to trade out more cards than are available");

    resourceCounts[static_cast<size_t>(inType)] += inCount;
    resourceCounts[static_cast<size_t>(outType)] -= outCount;
}

bool Bank::developmentCardAvailable() {
    return !developmentDeck.empty();
}

Card::Development Bank::drawDevelopmentCard() {
    if (developmentDeck.empty()) throw std::runtime_error("tried to draw development card on empty development card deck");

    auto topCard = developmentDeck.front();
    developmentDeck.pop_front();
    return topCard;
}



} // end Catan namespace