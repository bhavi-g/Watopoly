// test-action.cc
import <iostream>;
import Board;
import GameController;
import Player;

int main() {
    std::cout << "=== NEEDLES HALL RNG TEST (20 Trials) ===\n\n";

    GameController controller;
    Board* board = new Board();
    controller.setBoard(board);

    Player* vyomm = new Player("Vyomm", "V");
    controller.addPlayer(vyomm);

    // Run 20 organic simulations
    for (int i = 1; i <= 20; ++i) {
        std::cout << "\n=== TEST " << i << " ===\n";
        std::cout << "[Setup] Move to 19, roll (1,2) to land on Needles Hall (pos 22)\n";
        vyomm->moveTo(19);
        controller.simulateTurn(vyomm, 1, 2);  // die1 + die2 = 3 → lands on 22
    }

    delete vyomm;
    delete board;

    std::cout << "\n=== END OF TEST ===\n";
    return 0;
}
