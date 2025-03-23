// test-mortgage.cc
import <iostream>;
import GameController;
import Board;
import Player;
import AcademicBuilding;
import LandAction;

int main() {
    std::cout << "=== TEST: Mortgage & Rent Logic ===\n\n";

    GameController controller;
    Board* board = new Board();
    controller.setBoard(board);

    Player* vyomm = new Player("Vyomm", "V");
    Player* bhavish = new Player("Bhavish", "B");

    controller.addPlayer(vyomm);
    controller.addPlayer(bhavish);

    auto* ev1 = dynamic_cast<AcademicBuilding*>(board->getSquareByName("EV1"));
    auto* ev2 = dynamic_cast<AcademicBuilding*>(board->getSquareByName("EV2"));
    auto* ev3 = dynamic_cast<AcademicBuilding*>(board->getSquareByName("EV3"));  // Target

    // Give full monopoly to Vyomm
    for (auto* ab : {ev1, ev2, ev3}) {
        ab->setOwnerToken(vyomm->getToken());
        vyomm->addProperty(ab->getName());
    }

    // Case 1: Mortgage with improvements → should fail
    ev3->addImprovement();
    bool mortgageWithImprovement = controller.mortgageBuilding(vyomm, ev3);
    ev3->removeImprovement();

    // Case 2: Mortgage blocked by other buildings having improvements
    ev1->addImprovement();
    bool blockLevelMortgage = controller.mortgageBuilding(vyomm, ev3);
    ev1->removeImprovement();  // Clean up

    // Case 3: Valid mortgage on clean EV3
    bool successMortgage = controller.mortgageBuilding(vyomm, ev3);

    // Case 4: Rent test — Bhavish lands on mortgaged EV3
    std::cout << "\n=== RENT TEST: Mortgaged Property ===\n";

    int beforeBhavish = bhavish->getMoney();
    int beforeVyomm = vyomm->getMoney();

    int ev3Pos = ev3->getPosition();
    bhavish->moveTo(ev3Pos);
    LandAction action = ev3->onLand(bhavish);

    if (action == LandAction::PayRent && ev3->isMortgaged()) {
        std::cout << "[PASS] EV3 is mortgaged. Rent should be skipped.\n";

        if (bhavish->getMoney() == beforeBhavish && vyomm->getMoney() == beforeVyomm) {
            std::cout << "[PASS] No rent transferred. ✅\n";
        } else {
            std::cout << "[FAIL] Rent was incorrectly transferred! ❌\n";
        }
    }

    // Case 5: Unmortgage without enough funds
    vyomm->setMoney(0);
    bool failUnmortgage = controller.unmortgageBuilding(vyomm, ev3);

    // Case 6: Unmortgage with enough funds
    vyomm->setMoney(1000);  // Should be enough
    bool successUnmortgage = controller.unmortgageBuilding(vyomm, ev3);

    // Final: Rent test after unmortgaging
    std::cout << "\n=== RENT TEST: After Unmortgaging ===\n";
    bhavish->moveTo(ev3Pos);

    int rentBeforeBhavish = bhavish->getMoney();
    int rentBeforeVyomm = vyomm->getMoney();

    LandAction action2 = ev3->onLand(bhavish);

    if (action2 == LandAction::PayRent) {
        int rent = ev3->calculateRent();
        std::cout << "Rent should be $" << rent << ". Charging it...\n";
        bhavish->pay(rent);
        vyomm->receive(rent);
    }

    int rentAfterBhavish = bhavish->getMoney();
    int rentAfterVyomm = vyomm->getMoney();

    if (rentAfterBhavish < rentBeforeBhavish && rentAfterVyomm > rentBeforeVyomm) {
        std::cout << "[PASS] Rent correctly applied after unmortgage.\n";
    } else {
        std::cout << "[FAIL] Rent not applied as expected.\n";
    }

    // Result summary
    auto res = [](bool b) { return b ? "[PASS]" : "[FAIL]"; };
    std::cout << "\n=== RESULT SUMMARY ===\n";
    std::cout << "Case 1: Mortgage w/ improvements:        " << res(!mortgageWithImprovement) << "\n";
    std::cout << "Case 2: Block has improvements:          " << res(!blockLevelMortgage) << "\n";
    std::cout << "Case 3: Successful mortgage:             " << res(successMortgage) << "\n";
    std::cout << "Case 5: Unmortgage without enough funds: " << res(!failUnmortgage) << "\n";
    std::cout << "Case 6: Successful unmortgage:           " << res(successUnmortgage) << "\n";

    delete vyomm;
    delete bhavish;
    delete board;
    return 0;
}
