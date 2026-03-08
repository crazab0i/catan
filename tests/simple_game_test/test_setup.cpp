
#include "catanConsts.hpp"
#include "game.hpp"

#include <iostream>
#include <format>

using namespace Catan;

int main() {
    Game game(4);

    // print the board
    game.startGame();

    game.printBoard();

    // set the game
    for (int i = 0; i < 4; ++i) {
        auto result = game.rollForFirstPlayer();
        std::cout << std::format("Player {} rolled a {}, player with highest roll: {} with value {}\n", result.playerRolled, result.dieTotal, result.highestPlayerRolled, result.highestRolled);
    }

    std::cout << std::format("First player to go is {}", game.getFirstPlayer());

    // first round
    for (int i = 0; i < 4; ++i) {
        Board::PointID placePoint;
        std::cout << std::format("First Round: place settlement for player {}\n", game.getCurrentPlayer());
        std::cin >> placePoint;

        Board::EdgeID placeEdge;
        std::cout << std::format("\nFirst Round: place road for player {}\n", game.getCurrentPlayer());
        std::cin >> placeEdge;

        std::cout << "\n";

        const auto result = game.setupBuild({game.getCurrentPlayer(), Board::BuildingType::Settlement, {Board::BuildLocationTypes::Point, placePoint}}, {game.getCurrentPlayer(), Board::BuildingType::Road, {Board::BuildLocationTypes::Edge, placeEdge}});
        if (result.port) std::cout << std::format("Acquired port: {}\n", Board::portTypeToString(result.port.value()));
    }

    // second round
    for (int i = 0; i < 4; ++i) {
        Board::PointID placePoint;
        std::cout << std::format("Second Round: place settlement for player {}\n", game.getCurrentPlayer());
        std::cin >> placePoint;

        Board::EdgeID placeEdge;
        std::cout << std::format("\nFirst Second: place road for player {}\n", game.getCurrentPlayer());
        std::cin >> placeEdge;

        std::cout << "\n";

        const auto result = game.setupBuild({game.getCurrentPlayer(), Board::BuildingType::Settlement, {Board::BuildLocationTypes::Point, placePoint}}, {game.getCurrentPlayer(), Board::BuildingType::Road, {Board::BuildLocationTypes::Edge, placeEdge}});
        if (result.port) std::cout << std::format("Acquired port: {}\n", Board::portTypeToString(result.port.value()));
        std::cout << Economy::resourceArrayToString(result.gained_resources.value()) << "\n";
    }
}