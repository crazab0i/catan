
#include "game.hpp"
#include "bank.hpp"
#include "board.hpp"

#include <random>
#include <utility>

namespace Catan {

//////////////////////////////////////////////////////////////////////////
//
//      Game Class
//
/////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    //
    //      Private
    //
    /////////////////////////////////////////////////////////////////////////

std::pair<GameDefs::DieVal, GameDefs::DieVal> Game::_rollDie() {

    std::uniform_int_distribution<int> dieChance(1, 6);
    return std::make_pair(dieChance(rng), dieChance(rng));
}

const GameDefs::PlayerID Game::_turnToPlayerID(GameDefs::Turn turn) const {
    switch (turn) {
        case GameDefs::Turn::Player0: return 0;
        case GameDefs::Turn::Player1: return 1;
        case GameDefs::Turn::Player2: return 2;
        case GameDefs::Turn::Player3: return 3;
    }
}


    //////////////////////////////////////////////////////////////////////////
    //
    //      Public
    //
    /////////////////////////////////////////////////////////////////////////

void Game::startGame() {
    board.createBoard();
    started = true;
}

Game::Game(int numPlayers) : rng(std::random_device{}()), numPlayers(numPlayers) {
    players.reserve(numPlayers);
    
    for (int i = 0; i < numPlayers; ++i) {
        players.emplace_back(i);
    }
}

const GameDefs::PlayerID Game::getCurrentPlayer() const {
    return _turnToPlayerID(currentTurn);
}

void makeTrade(const Economy::CardCount inCount, const Card::Resource inType, const Economy::CardCount outCount, const Card::Resource outType, const TradeTarget target) {

}

void playDev(Card::Development card);
void playBuilding(Board::BuildingType building);

std::pair<GameDefs::DieVal, GameDefs::DieVal> Game::rollDie() {
    auto dieResult = _rollDie();
    GameDefs::DieVal dieTotal = dieResult.first + dieResult.second;

    auto payout = board.getRollPayout(dieTotal);
    
}



} // end Catan namespace