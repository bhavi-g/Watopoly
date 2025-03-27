// test-auction.cc
import GameController;
import Square;
import Player;
import Board;               // ✅ Use actual Board
import Building;
import <iostream>;

int main() {
    GameController gc;
    Board board;

    // === Hook board into controller so it populates buildings map ===
    gc.setBoard(&board);

    // === Create test players ===
    auto* p1 = new Player("Player1", "P1");
    auto* p2 = new Player("Player2", "P2");
    auto* p3 = new Player("Player3", "P3");

    p1->receive(500);
    p2->receive(500);
    p3->receive(500);

    gc.addPlayer(p1);
    gc.addPlayer(p2);
    gc.addPlayer(p3);

    // === Grab an actual building from the board ===
    Square* sq = board.getSquareByName("BMH");  // Health block
    auto* b = dynamic_cast<Building*>(sq);

    if (!b) {
        std::cout << "[Error] Could not find building 'BMH'.\n";
        return 1;
    }

    // === Trigger auction on a real board square ===
    gc.handleAuction(b);

    // === Show ownership result ===
    std::cout << "[Result] " << b->getName() << " is owned by: " << b->getOwnerToken() << "\n";

    // === Clean up players (Board owns squares) ===
    delete p1;
    delete p2;
    delete p3;

    return 0;
}
