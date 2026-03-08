#include "catanConsts.hpp"

#include <format>
#include <stdexcept>

namespace Catan {

const std::string Economy::resourceArrayToString(const ResourceArray &arr) {
    return std::format("Resource Array: Sheep: {}, Wood: {}, Wheat: {}, Brick: {}, Ore: {}", arr[0], arr[1], arr[2], arr[3], arr[4]);
}

void Board::Building::_checkPosition() const {
    if ((buildingType == BuildingType::Settlement || buildingType == BuildingType::City) && position.type != BuildLocationTypes::Point) {
        throw std::runtime_error("a city or settlement should be placed on a point");
    } else if (buildingType == BuildingType::Road && position.type != BuildLocationTypes::Edge) {
        throw std::runtime_error("a road should be build on a point");
    }
}

Board::Building::Building(const BuildPosition position) : position(position) {
    _checkPosition();
}

Board::Building::Building(const GameDefs::PlayerID owner, const BuildingType type, const BuildPosition position)
    : ownerID(owner), buildingType(type), position(position) {
    _checkPosition();
}

} // end Catan namespace