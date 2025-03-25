// test-slc.cc
import <iostream>;
import Board;
import GameController;
import Player;

int main() {
    std::cout << "=== SLC RNG TEST (15 Trials) ===\n\n";

    GameController controller;
    Board* board = new Board();
    controller.setBoard(board);

    Player* vyomm = new Player("Vyomm", "V");
    controller.addPlayer(vyomm);

    // Move Vyomm just before SLC (at pos 29)
    // A roll of (3,1) will land on pos 33 (SLC)
    for (int i = 1; i <= 15; ++i) {
        std::cout << "\n=== TEST " << i << " ===\n";
        std::cout << "[Setup] Move to 29, roll (3,1) to land on SLC (pos 33)\n";
        vyomm->moveTo(29);
        controller.playTurn(vyomm, std::pair{3, 1});
    }

    delete vyomm;
    delete board;

    std::cout << "\n=== END OF TEST ===\n";
    return 0;
}
