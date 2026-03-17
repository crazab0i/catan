
#pragma once

#include "catanConsts.hpp"

#include <array>
#include <optional>
#include <set>
#include <variant>
#include <vector>
#include <unordered_set>

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
        std::set<Board::PointID> validPoints;
        std::array<std::unordered_set<Board::PointID>, GameDefs::NUM_MAX_PLAYERS> validPlayerPoints;

        std::vector<Board::Edge> edges;
        std::set<Board::EdgeID> validEdges;
        std::array<std::unordered_set<Board::EdgeID>, GameDefs::NUM_MAX_PLAYERS> validPlayerEdges;

        std::array<std::optional<Board::PortType>, Board::NUM_POINTS> pointPorts;

        void _printTiles() const;
        void _printPoints() const;
        void _printEdges() const;
        void _printTileDieIndex() const;
        void _printPorts() const;

        Card::Resource _mapTileToCard(Board::TileType type) const;

        void _createTiles();
        void _createPointsAndEdges();
        void _createValidPointsAndEdges();
        void _createPortPoints();

        const std::set<Board::PointID> _getAdjacentPoints(const Board::PointID pointID) const;
        void _invalidatePointAndAdjacentPoints(const Board::PointID pointID);

        void _checkBuildingLocationIsValid(const Board::Building &building, const Board::BuildPhase phase) const;

        void _validateAdjacent(const Board::Building &building);

    public:
        void createBoard();
        void printBoard() const;
        
        GameBoard();

        const std::set<Board::PointID>& getValidPoints() const;
        const std::set<Board::EdgeID>& getValidEdges() const;

        const std::set<Board::PointID>& getValidPoints(GameDefs::PlayerID player) const;
        const std::set<Board::EdgeID>& getValidEdges(GameDefs::PlayerID player) const;

        using BuildDestination = std::variant<Board::TileID, Board::EdgeID>;
        std::optional<Board::PortType> placeBuilding(const Board::Building &building, const Board::BuildPhase phase);

        const std::vector<Board::TileID> getValidRobberDestinations() const;
        std::set<GameDefs::PlayerID> placeRobber(Board::TileID destination);

        // change this to std opt
        const std::array<Economy::PlayerPayout, Card::NUM_RESOURCE_TYPE> getRollPayout(const GameDefs::PlayerID dieVal) const;

        const Economy::ResourceArray getInitialSettlementPayout(const Board::PointID secondPlacement) const;

        Board::TileID getCurrentRobberPosition() const;
};



} // end Catan namespace