#include "bank.hpp"
#include "player.hpp"

using namespace Catan;

int main() {
    
    Player p1(1);
    
    p1.printPlayer();

    p1.recieveDevCard(Card::Development::Knight);
    p1.expendResources({1, 1, 1, 1, 0});
    p1.printPlayer();
}