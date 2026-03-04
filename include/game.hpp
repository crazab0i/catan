#pragma once

#include "player.hpp"
#include "board.hpp"
#include "bank.hpp"

#include <vector>

namespace Catan {

    constexpr int PLAYER_COUNT = 4;

class Game {
    private:
    
    std::vector<Player> players;
    GameBoard board;
    Bank bank;

    void _initialPhase();
    void _mainPhase();
    void _round();
    void _turn();

    void _rollDie();

    void _playAction();
    void _actionTrade();
    void _actionDevelopmentCard();
    void _actionBuild();

    void _checkWin();

    public:

    // need interface to have inputs and display

    void startGame();
    void setGame();

    Game();
};


} // end Catan namespace