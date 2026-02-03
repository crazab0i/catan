#include "bank.hpp"

#include <random>
#include <iostream>
#include <algorithm>

namespace Catan {

//////////////////////////////////////////////////////////////////////////
//
//      Bank Class
//
/////////////////////////////////////////////////////////////////////////

void Bank::_reset_resources() {
    for (int i = 0; i < Card::NUM_RESOURCE_TYPE; ++i) {
        resources[i] = Card::NUM_RESOURCE;
    }
}

void Bank::_reset_developmentDeck() {
    developmentDeck.clear();
    developmentDeck.reserve(Card::NUM_DEV_CARDS);

    for (auto [type, count] : Card::developmentCounts) {
        developmentDeck.insert(end(developmentDeck), count, type);
    }    

    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(begin(developmentDeck), end(developmentDeck), gen);
}

void Bank::_print_resources() {
    std::cout << "NUM_SHEEP: "  << resources[static_cast<size_t>(Card::Resource::Sheep)] << "\n";
    std::cout << "NUM_WOOD: "   << resources[static_cast<size_t>(Card::Resource::Wood)] << "\n";
    std::cout << "NUM_WHEAT: "  << resources[static_cast<size_t>(Card::Resource::Wheat)] << "\n";
    std::cout << "NUM_BRICK: "  << resources[static_cast<size_t>(Card::Resource::Brick)] << "\n";
    std::cout << "NUM_ORE: "    << resources[static_cast<size_t>(Card::Resource::Ore)] << "\n";
}

void Bank::_print_developmentDeck() {
    for (int i = Card::NUM_DEV_CARDS - 1; i >= 0; --i) {
        std::cout << Card::DevType_to_string(developmentDeck[i]);
    }
}

void Bank::reset() {
    _reset_resources();
    _reset_developmentDeck();
}

Bank::Bank() {
    reset();
}

} // end Catan namespace