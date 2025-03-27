import GameController;
import Player;
import Board;
import Building;
import <iostream>;

int main() {
    GameController gc;
    Board board;

    gc.setBoard(&board);

    // === Create players ===
    auto* p1 = new Player("Player1", "P1");
    auto* p2 = new Player("Player2", "P2");
    auto* p3 = new Player("Player3", "P3");

    // Give everyone money
    p1->receive(500);
    p2->receive(500);
    p3->receive(500);

    gc.addPlayer(p1);
    gc.addPlayer(p2);
    gc.addPlayer(p3);

    // === Move P1 right before BMH ===
    p1->moveTo(17);  // Position before BMH (which is at 18)

    // === Trigger playTurn with forced roll ===
    std::cout << "[Test] Simulating landing on unowned property BMH...\n";
    gc.playTurn(p1, std::pair{1, 0});  // Force dice roll to 1 step forward

    // === Check ownership result ===
    auto* bmh = dynamic_cast<Building*>(board.getSquareByName("BMH"));
    if (bmh) {
        std::cout << "[Result] " << bmh->getName()
                  << " is owned by: " << bmh->getOwnerToken() << "\n";
    }

    delete p1;
    delete p2;
    delete p3;

    return 0;
}
