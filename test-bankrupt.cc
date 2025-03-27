import GameController;
import Board;
import Player;
import AcademicBuilding;
import <iostream>;
import <vector>;

// Helper function to simulate player ownership + improvements.
void setupProperty(GameController& gc, Player* p, const std::string& name, int improveCount = 0) {
    auto* b = gc.getBuilding(name);
    b->setOwnerToken(p->getToken());
    p->addProperty(name);
    for (int i = 0; i < improveCount; ++i) {
        gc.improveBuilding(p, dynamic_cast<AcademicBuilding*>(b));
    }
}

// =======================
// === TEST SCENARIOS ===
// =======================

void testInsaneLiquidationToPlayer() {
    std::cout << "\n=== Test 6: Bankruptcy Avoided via Selling 20 Properties ===\n";
    Board board;
    GameController gc;
    gc.setBoard(&board);

    Player p1("Overextended", "OX");
    Player p2("Landlord", "LL");
    gc.addPlayer(&p1);
    gc.addPlayer(&p2);

    // Give p1 a comical amount of properties
    std::vector<std::string> properties = {
        "AL", "ML", "ECH", "PAS", "HH", "RCH", "DWE", "CPH", "LHI",
        "BMH", "OPT", "EV1", "EV2", "EV3", "PHYS", "B1", "B2", "EIT", "ESC", "C2"
    };

    for (const auto& name : properties) {
        setupProperty(gc, &p1, name);  // No improvements for now
    }

    // Setup p2 to own DC with rent ready
    setupProperty(gc, &p2, "DC");
    p2.setInTims(false);

    // Overextended is flat broke and will land on DC
    p1.setMoney(5);
    p1.moveTo(38);  // One step before DC

    // This should force the full liquidation logic to run across 20 props
    gc.playTurn(&p1, std::pair{1, 0});  // Lands on DC, owes $400
}


void testBankruptcyToPlayer() {
    std::cout << "\n=== Test 1: Bankruptcy to Another Player (Rent) ===\n";
    Board board;
    GameController gc;
    gc.setBoard(&board);

    Player p1("Player 1", "P1");
    Player p2("Player 2", "P2");
    gc.addPlayer(&p1);
    gc.addPlayer(&p2);

    setupProperty(gc, &p2, "DC");  // Expensive rent tile
    p1.setMoney(1);                // Not enough to pay rent
    p2.setInTims(false);

    p1.moveTo(38);  // One step before DC
    gc.playTurn(&p1, std::pair{1, 0}); // Lands on DC
}

void testBankruptcyToBank() {
    std::cout << "\n=== Test 2: Bankruptcy to Bank (Tuition Square) ===\n";
    Board board;
    GameController gc;
    gc.setBoard(&board);

    Player p("Solo", "S");
    gc.addPlayer(&p);

    p.setMoney(100);    // Not enough for $300 flat tuition
    p.moveTo(3);   // Tuition is at position 4
    gc.playTurn(&p, std::pair{1, 0});
}

void testLiquidationMenu() {
    std::cout << "\n=== Test 3: Liquidation Flow (Sell Improvements → Mortgage) ===\n";
    Board board;
    GameController gc;
    gc.setBoard(&board);

    Player p("Liquidator", "L");
    Player creditor("Creditor", "C");
    gc.addPlayer(&p);
    gc.addPlayer(&creditor);

    setupProperty(gc, &p, "AL", 5);
    setupProperty(gc, &p, "ML", 5);
    setupProperty(gc, &p, "MKV");

    p.setMoney(50);  // Not enough to pay
    gc.enforcePayment(&p, 1000, &creditor);  // Triggers liquidation
}

void testBankruptcyAfterImprovements() {
    std::cout << "\n=== Test 4: Bankruptcy after Total Liquidation ===\n";
    Board board;
    GameController gc;
    gc.setBoard(&board);

    Player p("Struggler", "S");
    gc.addPlayer(&p);

    setupProperty(gc, &p, "BMH", 5);
    setupProperty(gc, &p, "OPT", 5);

    p.setMoney(50);  // Too low for redemption
    gc.enforcePayment(&p, 3000);  // Should be unrecoverable
}

void testAuctionPostBankruptcy() {
    std::cout << "\n=== Test 5: Auction Trigger After Declined Purchase ===\n";
    Board board;
    GameController gc;
    gc.setBoard(&board);

    Player p("PoorBuyer", "PB");
    Player other("Richie", "R");
    gc.addPlayer(&p);
    gc.addPlayer(&other);

    p.setMoney(10);      // Can't buy AL
    other.setMoney(1000); // Rich backup
    p.moveTo(0);    // Start
    gc.playTurn(&p, std::pair{1, 0});  // Lands on AL
}

// Main driver for all bankruptcy-related tests
int main() {
    testInsaneLiquidationToPlayer();
    testBankruptcyToPlayer();
    testBankruptcyToBank();
    testLiquidationMenu();
    testBankruptcyAfterImprovements();
    testAuctionPostBankruptcy();
    return 0;
}
