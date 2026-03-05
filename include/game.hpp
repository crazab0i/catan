#pragma once

#include "catanConsts.hpp"
#include "player.hpp"
#include "board.hpp"
#include "bank.hpp"

#include <random>

namespace Catan {

class Game {
    private:
    std::mt19937 rng;

    bool started = false;

    std::vector<Player> players;
    int numPlayers;

    GameBoard board;
    Bank bank;

    GameDefs::Stage currentStage = GameDefs::Stage::Setup;
    GameDefs::Turn currentTurn;
    GameDefs::TurnStage currentTurnStage = GameDefs::TurnStage::Roll;

    bool devPlayed = false;

    std::pair<GameDefs::DieVal, GameDefs::DieVal> _rollDie();

    const GameDefs::PlayerID _turnToPlayerID(GameDefs::Turn turn) const;
    public:

    // need interface to have inputs and display



    Game(int numPlayers);

    void startGame();

    // outside interface to control game

    const GameDefs::PlayerID getCurrentPlayer() const;

    void makeTrade(const Economy::CardCount inCount, const Card::Resource inType, const Economy::CardCount outCount, const Card::Resource outType, const Economy::TradeTarget target);

    void playDev(Card::Development card);
    void playBuilding(Board::BuildingType building);

    std::pair<GameDefs::DieVal, GameDefs::DieVal> rollDie();

    void endTurn();
};


} // end Catan namespace