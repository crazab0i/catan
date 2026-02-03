#include "board.hpp"

#include <iostream>
#include <algorithm>
#include <random>

namespace Catan {

namespace Board {


//////////////////////////////////////////////////////////////////////////
//
//      STRUCTS
//
/////////////////////////////////////////////////////////////////////////

Point::Point(int id, int tile_id) : id(PointID(id)), tile_id(TileID(tile_id)) {}
Edge::Edge(int id) : id(EdgeID(id)) {}
Tile::Tile(bool hasRobber, 
           int id, 
           int diceValue,
           TileType type) : hasRobber(hasRobber), id(id), diceValue(diceValue), type(type) {}

void Tile::printTile() {
    std::cout << "tile id: " << static_cast<int>(id);
    std::cout << "\ntile type: " << TileType_to_string(type);
    std::cout << "\ndice value: " << diceValue;
    std::cout << "\nhas robber: " << hasRobber;
    std::cout << "\npoints: ";
    for (const auto point : tilePoints[id]) {
        std::cout << static_cast<int>(point) << ", ";
    }
    std::cout << "\ndeges: ";
    for (const auto edge : tileEdges[id]) {
        std::cout << static_cast<int>(edge) << ", ";
    }
}

} // end Board namespace

//////////////////////////////////////////////////////////////////////////
//
//      Board Class
//
/////////////////////////////////////////////////////////////////////////


void GameBoard::createBoard() {
    std::vector<Board::TileType> types;
    types.reserve(Board::NUM_TILES);

    for (auto [type, count] : Board::tileCounts) {
        types.insert(types.end(), count, type);
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(begin(types), end(types), gen);

    std::vector<int> dice_values = {5, 2, 6, 3, 8, 10, 9, 12, 11, 4, 8, 10, 9, 4, 5, 6, 3, 11};
    auto dice_it = begin(dice_values);

    tiles.clear();
    tiles.reserve(Board::NUM_TILES);
    for (size_t i = 0; i < types.size(); ++i) {
        
        if (types[i] == Board::TileType::Desert) {
            tiles.emplace_back(true, i, -1, types[i]);
        } else {
            tiles.emplace_back(false, i, *dice_it++, types[i]);
        }
    }

}

void GameBoard::printBoard() {
    for (int i = 0; i < Board::NUM_TILES; ++i) {
        tiles.at(i).printTile();
    }
}

GameBoard::GameBoard() {
    createBoard();
}

} // end Catan namespace