#pragma once

#include "bank.hpp"
#include "board.hpp"
#include "catanConsts.hpp"
#include "player.hpp"

#include <random>

namespace Catan {

class Game {
    private:

    std::vector<Player> players;
    int numPlayers;

    GameBoard board;
    Bank bank;

    std::mt19937 rng;

    // state

    GameDefs::PlayerID holderOfLongestRoad = Board::NO_OWNER;
    GameDefs::PlayerID holderOfLargestArmy = Board::NO_OWNER;

    bool inFinalRound = false;

    GameDefs::SetupState setupState;

    GameDefs::Stage currentStage = GameDefs::Stage::Start;

    GameDefs::Turn currentTurn;
    GameDefs::TurnStage currentTurnStage = GameDefs::TurnStage::None;

    bool devPlayed = false;

    std::pair<GameDefs::DieVal, GameDefs::DieVal> _rollDie();

    GameDefs::PlayerID _turnToPlayerID(GameDefs::Turn turn) const;

    void _checkTurn(const GameDefs::TurnStage expectedTurnStage) const;
    void _checkSetup(const GameDefs::SetupStage expectedSetupStage) const;


    void _checkStage(const GameDefs::Stage expectedStage) const;
    void _checkTurnStage(const GameDefs::TurnStage expectedTurnStage) const;
    void _checkSetupStage(const GameDefs::SetupStage expectedSetupStage) const;

    void _checkBuilding(const Board::Building &building, const Board::BuildingType expectedType) const;

    void _setTurnNextPlayer(const bool forward = true);

    public:

    // need interface to have inputs and display


    Game(int numPlayers);

    // start stage

    void startGame();

    // outside interface to control game

    // setup stage

    const API::FirstRollResult rollForFirstPlayer();
    const API::SetupBuildResult setupBuild(const Board::Building &settlement, const Board::Building &road);

    // main stage

    const API::RollResult rollDie();

    void endTurn();

    // general

    GameDefs::PlayerID getFirstPlayer() const;
    GameDefs::PlayerID getCurrentPlayer() const;

    Board::TileID getRobberPosition() const;
    
    // miscell print functions

    void printBank() const;
    void printBoard() const;
    void printPlayer(const GameDefs::PlayerID playerID) const;
};


} // end Catan namespace