#pragma once

#include "board.hpp"
#include "bank.hpp"

namespace Catan {

class Player {
    private:

    // sheep, wood, wheat, brick, ore. Each player starts with enough to build 2 settlements and 2 roads
    std::array<int, static_cast<int>(Card::Resource::_Count)> resource_inventory_count = {2, 4, 2, 4, 0};


    public:
    Player();
};


} // end Catan namespace