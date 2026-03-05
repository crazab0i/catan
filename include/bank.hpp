#pragma once

#include "catanConsts.hpp"

#include <deque>

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

class Bank {
    private:

        Economy::ResourceArray resourceCounts;

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

        const Economy::ResourceArray& getResourceCounts() const;

        bool dealCards(const Economy::PlayerPayout &playerPayout, const Card::Resource type);
        void buyBuildable(const Economy::Buildable type);
        void trade(const Economy::CardCount inCount, const Economy::CardCount outCount, const Card::Resource inType, const Card::Resource outType);
        
        bool developmentCardAvailable();
        Card::Development drawDevelopmentCard();
        void discardDevelopmentCard();
};
} // end Catan namespace