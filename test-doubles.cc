// test-double-turns.cc
import <iostream>;
import GameController;
import Board;
import Player;

int main() {
    std::cout << "=== DOUBLE ROLL TURN SIMULATION ===\n\n";

    GameController controller;
    Board* board = new Board();
    controller.setBoard(board);

    Player* vyomm = new Player("Vyomm", "V");
    Player* bhavish = new Player("Bhavish", "B");

    controller.addPlayer(vyomm);
    controller.addPlayer(bhavish);

    // === Vyomm: Double → Double → Normal ===
    std::cout << "\n=== VYOMM'S TURN: Double → Double → Normal ===\n";
    vyomm->moveTo(0);

    // First double
    controller.playTurn(vyomm, std::pair{2, 2});  // triggers bonus turn 1

    // BONUS TURN 1: another double
    // [input]: 3 3 → triggers bonus turn 2

    // BONUS TURN 2: normal roll
    // [input]: 1 2 → ends streak

    // === Bhavish: Double → Double → Double → Jail ===
    std::cout << "\n=== BHAVISH'S TURN: 3 Doubles → Goes to Jail ===\n";
    bhavish->moveTo(0);

    controller.playTurn(bhavish, std::pair{1, 1});  // double 1
    // [input]: 2 2 → double 2
    // [input]: 3 3 → double 3 → jail

    delete vyomm;
    delete bhavish;
    delete board;

    std::cout << "\n=== END OF TEST ===\n";
    return 0;
}
