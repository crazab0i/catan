#pragma once

#include "catanConsts.hpp"
#include "player.hpp"
#include "board.hpp"
#include "bank.hpp"

#include <random>

namespace Catan {

class Game {
    private:

    GameDefs::SetupState setupState;

    GameDefs::PlayerID holderOfLongestRoad = Board::NO_OWNER;
    GameDefs::PlayerID holderOfLargestArmy = Board::NO_OWNER;

    std::mt19937 rng;

    bool inFinalRound = false;

    std::vector<Player> players;
    int numPlayers;

    GameBoard board;
    Bank bank;

    GameDefs::Stage currentStage = GameDefs::Stage::Setup;
    GameDefs::Turn currentTurn;
    GameDefs::TurnStage currentTurnStage = GameDefs::TurnStage::None;

    bool devPlayed = false;

    std::pair<GameDefs::DieVal, GameDefs::DieVal> _rollDie();

    const GameDefs::PlayerID _turnToPlayerID(GameDefs::Turn turn) const;

    void _checkTurn(const GameDefs::TurnStage expectedTurnStage) const;
    void _checkSetup(const GameDefs::SetupStage expectedSetupStage) const;


    void _checkStage(const GameDefs::Stage expectedStage) const;
    void _checkTurnStage(const GameDefs::TurnStage expectedTurnStage) const;
    void _checkSetupStage(const GameDefs::SetupStage expectedSetupStage) const;

    public:

    // need interface to have inputs and display


    Game(int numPlayers);

    // start stage

    void startGame();

    // outside interface to control game

    // setup stage

    const API::FirstRollResult rollForFirstPlayer();

    // main stage

    const API::RollResult rollDie();

    void endTurn();

    // general

    const GameDefs::PlayerID getFirstPlayer() const;
    const GameDefs::PlayerID getCurrentPlayer() const;


};


} // end Catan namespace