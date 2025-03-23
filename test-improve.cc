// test-improve.cc
import <iostream>;
import GameController;
import Board;
import Player;
import AcademicBuilding;
import LandAction;

int main() {
    std::cout << "=== TEST: Academic Building Improvement Edge Cases ===\n\n";

    GameController controller;
    Board* board = new Board();
    controller.setBoard(board);

    Player* vyomm = new Player("Vyomm", "V");
    Player* bhavish = new Player("Bhavish", "B");
    controller.addPlayer(vyomm);
    controller.addPlayer(bhavish);

    // Grab all buildings in EV block
    auto* ev1 = dynamic_cast<AcademicBuilding*>(board->getSquareByName("EV1"));
    auto* ev2 = dynamic_cast<AcademicBuilding*>(board->getSquareByName("EV2"));
    auto* ev3 = dynamic_cast<AcademicBuilding*>(board->getSquareByName("EV3"));  // Target

    // Set up true monopoly for Vyomm
    for (auto* ab : {ev1, ev2, ev3}) {
        ab->setOwnerToken(vyomm->getToken());
        vyomm->addProperty(ab->getName());
    }

    // ----- CASE 1: Valid Improvement -----
    bool case1 = controller.improveBuilding(vyomm, ev3);

    // ----- CASE 2: Bhavish tries to improve -----
    bool case2 = controller.improveBuilding(bhavish, ev3);

    // ----- CASE 3: Break monopoly -----
    ev2->setOwnerToken("B");
    bool case3 = controller.improveBuilding(vyomm, ev3);
    ev2->setOwnerToken("V"); // Restore monopoly

    // ----- CASE 4: Exceed max improvements -----
    for (int i = ev3->getImprovementCount(); i < 5; ++i) {
        ev3->addImprovement();
    }
    bool case4 = controller.improveBuilding(vyomm, ev3);

    // ----- CASE 5: Not enough money -----
    vyomm->setMoney(0);
    bool case5 = controller.improveBuilding(vyomm, ev3);

    // ----- Result summary -----
    auto result = [](bool passed) {
        return passed ? "[PASS]" : "[FAIL]";
    };

    std::cout << "\n===== TEST RESULTS =====\n";
    std::cout << "Case 1 - Valid Improvement:      " << result(case1) << "\n";
    std::cout << "Case 2 - Wrong Player:           " << result(!case2) << "\n";
    std::cout << "Case 3 - Broken Monopoly:        " << result(!case3) << "\n";
    std::cout << "Case 4 - Max Improvements:       " << result(!case4) << "\n";
    std::cout << "Case 5 - Not Enough Funds:       " << result(!case5) << "\n";

    // ----- Bonus: Bhavish lands on improved EV3 -----
    std::cout << "\n===== RENT TEST =====\n";
    int ev3Pos = ev3->getPosition();
    bhavish->moveTo(ev3Pos);

    std::cout << "Bhavish lands on " << ev3->getName() << " (owned by Vyomm, "
              << ev3->getImprovementCount() << " improvements)\n";

    LandAction action = ev3->onLand(bhavish);
    if (action == LandAction::PayRent) {
        int rent = ev3->calculateRent();  // context not needed here
        std::cout << "[Controller]: Bhavish must pay $" << rent << " in rent.\n";
        bhavish->pay(rent);
        vyomm->receive(rent);
        std::cout << "[After] Bhavish: $" << bhavish->getMoney()
                  << " | Vyomm: $" << vyomm->getMoney() << "\n";
    } else {
        std::cout << "Unexpected LandAction returned!\n";
    }

std::cout << "\n===== DEGRADE TEST =====\n";

for (int i = 1; i <= 6; ++i) {
    std::cout << "\n-- Attempt " << i << " to degrade EV3 --\n";
    bool degradeOk = controller.degradeBuilding(vyomm, ev3);
    std::cout << "Degrade result: " << (degradeOk ? "[PASS]" : "[FAIL]") << "\n";
    std::cout << "EV3 now has " << ev3->getImprovementCount() << " improvements.\n";
}


    // Cleanup
    delete vyomm;
    delete bhavish;
    delete board;

    return 0;
}
