
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

    std::cout << std::format("First player to go is {}\n", game.getFirstPlayer());

    // first round
    for (int i = 0; i < 4; ++i) {
        int placePoint;

        const auto valid = game.getValidSetupBuildLocations();
        std::cout << "Valid Points: ";
        for (const auto point : valid.validPoints) {
            std::cout << std::format("{} ", point);
        }
        std::cout << "\nValid Edges: ";
        for (const auto edge : valid.validEdges) {
            std::cout << std::format("{} ", edge);
        }

        std::cout << std::format("\nFirst Round: place settlement for player {}\n", game.getCurrentPlayer());
        std::cin >> placePoint;
        std::cout << std::format("point with val: {}\n", placePoint);

        int placeEdge;
        std::cout << std::format("\nFirst Round: place road for player {}\n", game.getCurrentPlayer());
        std::cin >> placeEdge;
        std::cout << std::format("edge with val: {}\n", placeEdge);

        std::cout << "\n";

        const auto result = game.setupBuild({game.getCurrentPlayer(), Board::BuildingType::Settlement, {Board::BuildLocationTypes::Point, static_cast<Board::PointID>(placePoint)}}, {game.getCurrentPlayer(), Board::BuildingType::Road, {Board::BuildLocationTypes::Edge, static_cast<Board::EdgeID>(placeEdge)}});
        if (result.port) std::cout << std::format("Acquired port: {}\n", Board::portTypeToString(result.port.value()));
    }

    // second round
    for (int i = 0; i < 4; ++i) {
        int placePoint;

        const auto valid = game.getValidSetupBuildLocations();
        std::cout << "Valid Points: ";
        for (const auto point : valid.validPoints) {
            std::cout << std::format("{} ", point);
        }
        std::cout << "\nValid Edges: ";
        for (const auto edge : valid.validEdges) {
            std::cout << std::format("{} ", edge);
        }
        std::cout << std::format("\nSecond Round: place settlement for player {}\n", game.getCurrentPlayer());
        std::cin >> placePoint;
        std::cout << std::format("point with val: {}", placePoint);


        int placeEdge;
        std::cout << std::format("\nFirst Second: place road for player {}\n", game.getCurrentPlayer());
        std::cin >> placeEdge;
        std::cout << std::format("edge with val: {}\n", placeEdge);

        std::cout << "\n";

        const auto result = game.setupBuild({game.getCurrentPlayer(), Board::BuildingType::Settlement, {Board::BuildLocationTypes::Point, static_cast<Board::PointID>(placePoint)}}, {game.getCurrentPlayer(), Board::BuildingType::Road, {Board::BuildLocationTypes::Edge, static_cast<Board::EdgeID>(placeEdge)}});
        if (result.port) std::cout << std::format("Acquired port: {}\n", Board::portTypeToString(result.port.value()));
        std::cout << Economy::resourceArrayToString(result.gained_resources.value()) << "\n";
    }
}