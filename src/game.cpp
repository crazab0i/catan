
#include "game.hpp"
#include "bank.hpp"
#include "board.hpp"
#include "catanConsts.hpp"

#include <random>
#include <stdexcept>
#include <utility>
#include <format>

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
        default:                      return Board::NO_OWNER;
    }
}

void Game::_checkTurn(const GameDefs::TurnStage expectedTurnStage) const {
    _checkStage(GameDefs::Stage::Main);
    _checkTurnStage(expectedTurnStage);
}

void Game::_checkSetup(const GameDefs::SetupStage expectedSetupState) const {
    _checkStage(GameDefs::Stage::Setup);
    _checkSetup(expectedSetupState);
}

void Game::_checkStage(const GameDefs::Stage expectedStage) const {
    if (expectedStage != currentStage) 
        throw std::runtime_error(std::format("expected stage to be: {}, found stage to be {}", GameDefs::stageToString(expectedStage), GameDefs::stageToString(currentStage)));
}

void Game::_checkTurnStage(const GameDefs::TurnStage expectedTurnStage) const {
    if (expectedTurnStage != currentTurnStage)
        throw std::runtime_error(std::format("expected turnStage to be: {}, found turnStage to be {}", GameDefs::turnStageToString(expectedTurnStage), GameDefs::turnStageToString(currentTurnStage)));
}

void Game::_checkSetupStage(const GameDefs::SetupStage expectedSetupStage) const {
    if (expectedSetupStage != setupState.stage)
        throw std::runtime_error(std::format("expected setupStage to be: {}, found setupStage to be {}", GameDefs::setupStateToString(expectedSetupStage), GameDefs::setupStateToString(setupState.stage)));
}

    //////////////////////////////////////////////////////////////////////////
    //
    //      Public
    //
    /////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        //
        //      Start Stage
        //
        /////////////////////////////////////////////////////////////////////////

void Game::startGame() {
    _checkStage(GameDefs::Stage::Start);

    board.createBoard();
    currentStage = GameDefs::Stage::Setup;
}

Game::Game(int numPlayers) : rng(std::random_device{}()), numPlayers(numPlayers) {
    players.reserve(numPlayers);
    
    for (int i = 0; i < numPlayers; ++i) {
        players.emplace_back(i);
    }
}

        //////////////////////////////////////////////////////////////////////////
        //
        //      Setup Stage
        //
        /////////////////////////////////////////////////////////////////////////

const API::FirstRollResult Game::rollForFirstPlayer() {

    _checkSetup(GameDefs::SetupStage::RollForFirstPlayer);

    const auto diceRoll = _rollDie();
    const GameDefs::DieVal dieTotal = diceRoll.first + diceRoll.second;
    const auto currentPlayer = getCurrentPlayer();

    if (dieTotal > setupState.highestRoll) {
        setupState.highestRoll = dieTotal;
        setupState.highestRollPlayer = currentPlayer;
    }

    if (static_cast<int>(currentPlayer) == numPlayers - 1) {
        setupState.firstPlayer = setupState.highestRollPlayer;
        setupState.lastPlayer = (setupState.firstPlayer + numPlayers - 1) % numPlayers;
        setupState.stage = GameDefs::SetupStage::FirstSettlementPlacement;
    }

    return { diceRoll, dieTotal, currentPlayer, setupState.highestRoll, setupState.highestRollPlayer };
}

        //////////////////////////////////////////////////////////////////////////
        //
        //      Main Stage
        //
        /////////////////////////////////////////////////////////////////////////

const API::RollResult Game::rollDie() {

    _checkTurn(GameDefs::TurnStage::Roll);

    std::array<std::optional<Economy::PlayerPayout>, Card::NUM_RESOURCE_TYPE> opt_payouts;

    auto dieResult = _rollDie();
    GameDefs::DieVal dieTotal = dieResult.first + dieResult.second;

    if (dieTotal == GameDefs::ROBBER_ROLL) currentTurnStage = GameDefs::TurnStage::MoveRobber;

    auto payouts = board.getRollPayout(dieTotal);
    for (size_t i = 0; i < Card::NUM_RESOURCE_TYPE; ++i) {
        if (bank.dealCards(payouts[i], static_cast<Card::Resource>(i))) {
            opt_payouts[i] = payouts[i];
        }
    }
    
    return { dieResult, dieTotal, opt_payouts, getCurrentPlayer() };
}


const GameDefs::PlayerID Game::getCurrentPlayer() const {
    return _turnToPlayerID(currentTurn);
}


} // end Catan namespace