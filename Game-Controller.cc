// GameController.cc (interface)
// Module: GameController
// Description:
//   Acts as the central mediator/controller in the Watopoly architecture.
//   Responsible for managing all players and buildings, coordinating trades,
//   and providing access to internal game entities.
//
//   GameController eliminates the need for direct Player ↔ Building references
//   by serving as the mediator in all game logic interactions.
//
//   This structure follows the Mediator design pattern.
//
// Related Modules:
//   - Player (stores player state and assets)
//   - Building (represent ownable squares)
//   - Board (uses Square*, GameController operates over Building*)
//   - Game logic (e.g., trade, rent, purchase) is centralized here

export module GameController;

import LandAction;
import <string>;
import <map>;
import <iostream>;
import <optional>;
import <utility>;
import Player;
import Building;
import Residence;
import AcademicBuilding;
import Gym;
import Board;
import new_Display;

export class GameController {
private:
    // Maps player token strings to Player* instances
    std::map<std::string, Player*> players;

    // Maps building names to Building* instances
    std::map<std::string, Building*> buildings;
    Board* board;  // NEW: pointer to the board

public:
    // Registers a player with the controller (must be unique token).
    void addPlayer(Player* p);

    // Registers a building with the controller (must be unique name).
    void addBuilding(Building* b);

    /**
     * Handles property-for-property or money-for-property trades.
     *
     * Example calls:
     *   trade("V", "DC", "B", "MC");       // property for property
     *   trade("B", "500", "V", "EV1");     // money for property
     *
     * Note: Only works if ownership and building conditions are valid.
     */
    void trade(const std::string& fromToken, const std::string& giveBuilding,
               const std::string& toToken, const std::string& receiveBuilding);

    // Retrieves a Player pointer by token string (e.g., "V", "B").
    Player* getPlayer(const std::string& token) const;

    // Retrieves a Building pointer by name (e.g., "DC", "EV1").
    Building* getBuilding(const std::string& name) const;

    void setBoard(Board* b);  // Inject the board from main

    void playTurn(Player* p, std::optional<std::pair<int, int>> forcedDice = std::nullopt);


    void promptPurchase(Player* p, Building* b);

    int getResidenceCount(const std::string& ownerToken) const;
    int getGymCount(const std::string& ownerToken) const;

    bool improveBuilding(Player* p, AcademicBuilding* ab);

    bool hasMonopoly(const std::string& token, const std::string& blockName);

    bool degradeBuilding(Player* p, AcademicBuilding* ab);  // Sell an improvement

    bool mortgageBuilding(Player* p, Building* b);
    bool unmortgageBuilding(Player* p, Building* b);

    void simulateTurn(Player* p, int die1, int die2);
    void handleAuction(Building* b);
    void declareBankruptcy(Player* debtor, Player* creditor);
    bool enforcePayment(Player* debtor, int amount, Player* creditor = nullptr);
    bool attemptToRaiseFunds(Player* p, int amountOwed);

};
