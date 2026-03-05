
#pragma once

#include "catanConsts.hpp"

#include <array>
#include <unordered_set>
#include <variant>
#include <vector>
#include <optional>

//////////////////////////////////////////////////////////////////////////
//
//      Catan board has 19 total hexes, oriented 3,4,5,4,3
//          There are 8 ports
//          
//      Hex Counts:
//          4 Sheep
//          4 Wood
//          4 wheat
//          3 Brick
//          3 Ore
//          1 desert
//
//
//////////////////////////////////////////////////////////////////////////



namespace Catan {

class GameBoard {
    private:
        Board::TileID currentRobberTile = Board::NON_TILE;

        std::vector<Board::Tile> tiles;
        std::vector<std::vector<Board::TileID>> tilesDieIndex;

        std::vector<Board::Point> points;
        std::unordered_set<Board::PointID> validPoints;

        std::vector<Board::Edge> edges;
        std::unordered_set<Board::EdgeID> validEdges;

        std::array<std::optional<Board::PortType>, Board::NUM_POINTS> pointPorts;

        void _printTiles() const;
        void _printPoints() const;
        void _printEdges() const;
        void _printTileDieIndex() const;

        Card::Resource _mapTileToCard(Board::TileType type) const;

        void _createTiles();
        void _createPointsAndEdges();
        void _createValidPointsAndEdges();
        void _createPortPoints();

    public:
        void createBoard();
        void printBoard() const;
        
        GameBoard();

        const std::unordered_set<Board::PointID>& getValidPoints() const;
        const std::unordered_set<Board::EdgeID>& getValidEdges() const;

        using BuildDestination = std::variant<Board::TileID, Board::EdgeID>;
        std::optional<Board::PortType> placeBuilding(Board::Building &&building, const BuildDestination &destination);

        const std::vector<Board::TileID> getValidRobberDestinations() const;
        std::unordered_set<GameDefs::PlayerID> placeRobber(Board::TileID destination);

        // change this to std opt
        std::array<Economy::PlayerPayout, Card::NUM_RESOURCE_TYPE> getRollPayout(const GameDefs::PlayerID dieVal) const;

        const Economy::ResourceArray getInitialSettlementPayout(const Board::PointID secondPlacement) const;
};



} // end Catan namespace