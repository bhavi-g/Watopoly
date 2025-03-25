// test-jail.cc
import <iostream>;
import GameController;
import Board;
import Player;
import AcademicBuilding;
import <cstdlib>;
import <ctime>;
import <optional>;
import <utility>;

int main() {
    std::cout << "=== JAIL MECHANICS TEST ===\n\n";

    GameController controller;
    Board *board = new Board();
    controller.setBoard(board);

    Player *vyomm = new Player("Vyomm", "V");
    controller.addPlayer(vyomm);

    int die1, die2;

    // === CASE 0: Rent Skipped If Owner is in Jail ===
    std::cout << "\n=== CASE 0: Rent Skipped If Owner is in Jail ===\n";

    // Setup Bhavish and give them B2
    Player* bhavish = new Player("Bhavish", "B");
    controller.addPlayer(bhavish);
    auto* b2 = dynamic_cast<AcademicBuilding*>(board->getSquareByName("B2"));
    b2->setOwnerToken("B");
    bhavish->addProperty("B2");

    // Move Bhavish to 22 and roll (4,4) to land on 30 and go to jail
    std::cout << "[Setup] Bhavish moves to 22 and rolls (4,4) to land on GO TO TIMS.\n";
    bhavish->moveTo(22);
    controller.playTurn(bhavish, std::make_pair(4, 4));
    std::cout << "[ASSERT] Bhavish is in jail: " << (bhavish->isInTims() ? "[PASS]" : "[FAIL]") << "\n";

    // Vyomm moves to 23 and rolls (3,3) to land on 29 (B2)
    std::cout << "\n[Action] Vyomm moves to 23 and rolls (3,3) to land on B2 (owned by jailed Bhavish).\n";
    vyomm->moveTo(23);
    controller.playTurn(vyomm, std::make_pair(3, 3));  // Rent should be skipped

    // === Still CASE 0: Rent should now be collected after Bhavish is freed ===
    std::cout << "\n[Action] Bhavish is freed. Vyomm lands on B2 again.\n";
    bhavish->setInTims(false);
    vyomm->moveTo(23);
    controller.playTurn(vyomm, std::make_pair(3, 3));  // Rent should now be collected


    // === 1. Land on Go To Tims by simulating a move ===
    std::cout << "\n=== CASE 1: Land on Go To Tims ===\n";
    std::cout << "[Setup] Move to square 22 and input (die1 = 4, die2 = 4) to land on 30.\n";
    vyomm->moveTo(22);
    controller.playTurn(vyomm, std::pair{4, 4}); // lands on 30
    std::cout << "[ASSERT] Player should now be in jail: " << (vyomm->isInTims() ? "[PASS]" : "[FAIL]") << "\n";

    // === 2. First failed roll (no doubles) ===
    std::cout << "\n=== CASE 2: Fail T1 (no doubles) ===\n";
    std::cout << "[Input] Enter non-doubles like (4 5): ";
    std::cin >> die1 >> die2;
    controller.playTurn(vyomm, std::pair{die1, die2});

    // === 3. Second failed roll (no doubles) ===
    std::cout << "\n=== CASE 3: Fail T2 (no doubles) ===\n";
    std::cout << "[Input] Enter non-doubles like (2 3): ";
    std::cin >> die1 >> die2;
    controller.playTurn(vyomm, std::pair{die1, die2});

    // === 4. Third failed roll → forced pay + move ===
    std::cout << "\n=== CASE 4: Fail T3 → Forced Pay + Escape ===\n";
    std::cout << "[Input] Enter non-doubles like (1 5): ";
    std::cin >> die1 >> die2;
    controller.playTurn(vyomm, std::pair{die1, die2});
    std::cout << "[ASSERT] Player should now be free: " << (!vyomm->isInTims() ? "[PASS]" : "[FAIL]") << "\n";

    // === 5. Normal Turn → Doubles = Bonus Turn ===
    std::cout << "\n=== CASE 5: Normal Turn w/ Doubles → Bonus Turn ===\n";
    std::cout << "[Input] Enter doubles like (5 5): ";
    std::cin >> die1 >> die2;
    controller.playTurn(vyomm, std::pair{die1, die2});

    // === 6. Roll doubles and land on Go To Tims → No Extra Turn ===
    std::cout << "\n=== CASE 6: Doubles → Land on Go To Tims → No Extra Turn ===\n";
    std::cout << "[Setup] Move to 22 and enter (4 4) to land on 30.\n";
    vyomm->moveTo(22);
    std::cout << "[Input] Enter (4 4): ";
    std::cin >> die1 >> die2;
    controller.playTurn(vyomm, std::pair{die1, die2});
    std::cout << "[ASSERT] Player is in jail again: " << (vyomm->isInTims() ? "[PASS]" : "[FAIL]") << "\n";

    // === 7. Use Roll Up Cup to get out ===
    std::cout << "\n=== CASE 7: Use Roll Up the Rim Cup ===\n";
    vyomm->addRollUpCup();
    std::cout << "[Input] Enter any roll, e.g. (3 2): ";
    std::cin >> die1 >> die2;
    controller.playTurn(vyomm, std::pair{die1, die2});
    std::cout << "[ASSERT] Player used cup and is free: " << (!vyomm->isInTims() ? "[PASS]" : "[FAIL]") << "\n";

    // === 8. Re-enter Jail and pay $50 on first turn ===
    std::cout << "\n=== CASE 8: Re-enter Jail + Pay Early ===\n";
    std::cout << "[Setup] Move to 22 and enter (4 4) to land on 30.\n";
    vyomm->moveTo(22);
    controller.playTurn(vyomm, std::pair{4, 4});
    vyomm->setMoney(1000);
    std::cout << "[Input] Enter any roll (simulate pay flow): ";
    std::cin >> die1 >> die2;
    controller.playTurn(vyomm, std::pair{die1, die2});
    std::cout << "[ASSERT] Player paid and is free: " << (!vyomm->isInTims() ? "[PASS]" : "[FAIL]") << "\n";

    // === 9. Go to Jail via simulateTurn, then 4 playTurn()s ===
    std::cout << "\n=== CASE 9: Simulate Jail + Use Real playTurn() ===\n";
    std::cout << "[Setup] Move to 23, roll 3 + 4 to land on 30 and go to Tims.\n";
    vyomm->moveTo(23);
    controller.playTurn(vyomm, std::pair{3, 4});

    if (vyomm->isInTims()) {
        std::cout << "[ASSERT] In Jail after simulateTurn: [PASS]\n";
    } else {
        std::cout << "[ASSERT] In Jail after simulateTurn: [FAIL]\n";
    }

    vyomm->setMoney(1000);
    vyomm->addRollUpCup();

    std::cout << "\n[NOTE] Calling playTurn() 4 times now. Follow the prompts.\n";
    for (int i = 1; i <= 4; ++i) {
        std::cout << "\n--- Turn " << i << " ---\n";
        controller.playTurn(vyomm);  // No forced dice
    }

    delete vyomm;
    delete board;

    std::cout << "\n=== END OF JAIL TEST ===\n";
    return 0;
}
