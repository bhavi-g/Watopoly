// test-trade.cc
import <iostream>;
import Board;
import GameController;
import Player;
import AcademicBuilding;
import Residence;
import Gym;

void resetBoardState(GameController& controller, Player* vyomm, Player* bhavish) {
    // Reset player balance and jail state
    vyomm->setMoney(1500);
    bhavish->setMoney(1500);
    vyomm->setInTims(false);
    bhavish->setInTims(false);

    // Manually clear ownership of relevant properties
    std::string props[] = {"AL", "ML", "PAS", "DC", "CPH", "DWE", "ECH"};
    for (const std::string& name : props) {
        auto* bldg = controller.getBuilding(name);
        if (bldg) bldg->setOwnerToken("");
        if (auto* ab = dynamic_cast<AcademicBuilding*>(bldg)) {
            while (ab->getImprovementCount() > 0) {
                ab->removeImprovement();
            }
        }
    }
}

int main() {
    std::cout << "=== TRADE TESTS ===\n\n";

    GameController controller;
    Board* board = new Board();
    controller.setBoard(board);

    Player* vyomm = new Player("Vyomm", "V");
    Player* bhavish = new Player("Bhavish", "B");

    controller.addPlayer(vyomm);
    controller.addPlayer(bhavish);

    // === TEST 1: Valid Property-for-Property ===
    resetBoardState(controller, vyomm, bhavish);
    controller.getBuilding("AL")->setOwnerToken("V");
    vyomm->addProperty("AL");
    controller.getBuilding("DC")->setOwnerToken("B");
    bhavish->addProperty("DC");

    std::cout << "=== TEST 1: Valid Property-for-Property ===\n";
    controller.trade("V", "AL", "B", "DC");

    // === TEST 2: Invalid Player Name ===
    resetBoardState(controller, vyomm, bhavish);
    std::cout << "\n=== TEST 2: Invalid Player Name ===\n";
    controller.trade("X", "AL", "B", "DC");

    // === TEST 3: Invalid Property Name ===
    resetBoardState(controller, vyomm, bhavish);
    std::cout << "\n=== TEST 3: Invalid Property Name ===\n";
    controller.trade("V", "FOOBAR", "B", "DC");

    // === TEST 4: Ownership Mismatch ===
    resetBoardState(controller, vyomm, bhavish);
    controller.getBuilding("DC")->setOwnerToken("V");
    vyomm->addProperty("DC");
    std::cout << "\n=== TEST 4: Ownership Mismatch ===\n";
    controller.trade("V", "ECH", "B", "DC");

    // === TEST 5: Money for Property ===
    resetBoardState(controller, vyomm, bhavish);
    controller.getBuilding("DC")->setOwnerToken("B");
    bhavish->addProperty("DC");
    std::cout << "\n=== TEST 5: Money for Property ===\n";
    controller.trade("V", "500", "B", "DC");

    // === TEST 6: Property for Money ===
    resetBoardState(controller, vyomm, bhavish);
    controller.getBuilding("AL")->setOwnerToken("V");
    vyomm->addProperty("AL");
    std::cout << "\n=== TEST 6: Property for Money ===\n";
    controller.trade("V", "AL", "B", "300");

    // === TEST 7: Money for Money ===
    resetBoardState(controller, vyomm, bhavish);
    std::cout << "\n=== TEST 7: Money for Money (Invalid) ===\n";
    controller.trade("V", "400", "B", "200");

    // === TEST 8: Trade with Improvements ===
    resetBoardState(controller, vyomm, bhavish);
    auto* cph = dynamic_cast<AcademicBuilding*>(controller.getBuilding("CPH"));
    cph->setOwnerToken("V");
    vyomm->addProperty("CPH");
    cph->addImprovement();
    controller.getBuilding("DC")->setOwnerToken("B");
    bhavish->addProperty("DC");
    std::cout << "\n=== TEST 8: Trade with Improvements (Should Fail) ===\n";
    controller.trade("V", "CPH", "B", "DC");

    // === TEST 9: Trade within Improved Monopoly ===
    resetBoardState(controller, vyomm, bhavish);
    auto* dwe = dynamic_cast<AcademicBuilding*>(controller.getBuilding("DWE"));
    cph->setOwnerToken("V");
    dwe->setOwnerToken("V");
    vyomm->addProperty("CPH");
    vyomm->addProperty("DWE");
    dwe->addImprovement();
    controller.getBuilding("DC")->setOwnerToken("B");
    bhavish->addProperty("DC");
    std::cout << "\n=== TEST 9: Trade within Improved Monopoly (Should Fail) ===\n";
    controller.trade("V", "CPH", "B", "DC");

    // === TEST 10: Accepting Trade While in Jail ===
    resetBoardState(controller, vyomm, bhavish);
    bhavish->setInTims(true);
    controller.getBuilding("ML")->setOwnerToken("V");
    controller.getBuilding("DC")->setOwnerToken("B");
    vyomm->addProperty("ML");
    bhavish->addProperty("DC");
    std::cout << "\n=== TEST 10: Accepting Trade While in Jail ===\n";
    controller.trade("V", "ML", "B", "DC");

    delete board;
    delete vyomm;
    delete bhavish;

    std::cout << "\n=== END OF TRADE TESTS ===\n";
    return 0;
}
