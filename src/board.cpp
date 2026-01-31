#include "board.hpp"

namespace Catan {


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
    std::cout << "\ntile type: " << TileType_to_string(type) << "\n";
}


void Board::createBoard() {
    std::vector<TileType> types = {
        TileType::Sheep, TileType::Sheep, TileType::Sheep, TileType::Sheep,
        TileType::Wood, TileType::Wood, TileType::Wood, TileType::Wood,
        TileType::Wheat, TileType::Wheat, TileType::Wheat, TileType::Wheat,
        TileType::Brick, TileType::Brick, TileType::Brick,
        TileType::Ore, TileType::Ore, TileType::Ore,
        TileType::Desert,
    };

    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(begin(types), end(types), gen);

    std::vector<int> dice_values = {5, 2, 6, 3, 8, 10, 9, 12, 11, 4, 8, 10, 9, 4, 5, 6, 3, 11};
    auto dice_it = begin(dice_values);

    for (int i = 0; i < types.size(); ++i) {
        bool hasRobber = types[i] == TileType::Desert;
        
        tiles.emplace_back(hasRobber, i, dice_it++, types[i]);
    }

}

void Board::printBoard() {
    std::cout << "==================== Row: 1 ====================\n\n";
    for (int i = 0; i < 3; ++i) {
        tiles.at(i).printTile();
    }
    std::cout << "==================== Row: 2 ====================\n\n";
    for (int i = 3; i < 7; ++i) {
        tiles.at(i).printTile();
    }
    std::cout << "==================== Row: 3 ====================\n\n";
    for (int i = 7; i < 12; ++i) {
        tiles.at(i).printTile();
    }
    std::cout << "==================== Row: 4 ====================\n";
    for (int i = 12; i < 16; ++i) {
        tiles.at(i).printTile();
    }
    std::cout << "==================== Row: 5 ====================\n";
    for (int i = 16; i < 19; ++i) {
        tiles.at(i).printTile();
    }
}

} // end Catan namespace