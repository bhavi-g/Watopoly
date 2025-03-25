// Player.cc (interface)
// Module: Player
// Description:
//   Represents a human-controlled player in the game of Watopoly.
//   Each player has a unique token, a name, a money balance, and a collection
//   of buildings they currently own (tracked by building name).
//
//   This module is intentionally decoupled from Building to avoid circular
//   dependencies. Ownership is represented via building name strings.
//
// Related Modules:
//   - GameController (invokes player pay/receive, manages turns and interactions)
//   - Building (calls getToken() to check ownership)
//   - Square (players are passed to onLand())

export module Player;

import <string>;
import <vector>;

export class Player {
private:
    std::string name;                         // Player's name (e.g., "Vyomm")
    std::string token;                        // Unique identifier (e.g., "V")
    int money;                                // Player's current balance
    std::vector<std::string> ownedBuildings;  // List of names of owned properties
    int position = 0; // Default starting position
    bool inTims = false;
    int timsTurns = 0;
    int rollUpCups = 0;  // 🥤 "Get Out of Jail Free" cards


public:
    // Constructs a player with a given name, token, and optional starting money.
    Player(std::string name, std::string token, int startMoney = 1500);

    // Returns the player's name.
    std::string getName() const;

    // Returns the player's token (used to represent them on the board).
    std::string getToken() const;

    // Returns the player's current amount of money.
    int getMoney() const;

    // Deducts money from the player's balance.
    void pay(int amount);

    // Adds money to the player's balance.
    void receive(int amount);

    // Adds a building (by name) to the player's owned list.
    void addProperty(const std::string& buildingName);

    // Removes a building (by name) from the player's owned list.
    void removeProperty(const std::string& buildingName);

    // Checks whether the player owns a given building.
    bool owns(const std::string& buildingName) const;

    // Returns the current board position of the player.
    void move(int steps);

    // Moves the player forward by a number of steps with board wraparound.
    int getPosition() const;

    // Moves the player directly to a specific board position (no wrapping).
    void moveTo(int newPosition);

    void setMoney(int newAmount);

    bool isInTims() const;

    void setInTims(bool);

    int getTimsTurns() const;

    void incrementTimsTurn();

    void resetTimsTurns();

    int getRollUpCups() const;

    void addRollUpCup();
    
    void useRollUpCup();

};
