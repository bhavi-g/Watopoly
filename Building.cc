// Building.cc (interface)
// Module: Building
// Description:
//   Represents an ownable square on the Watopoly board.
//   This abstract layer includes core logic shared by AcademicBuilding,
//   Residence, and Gym.
//
//   All Buildings:
//     - Are derived from Square
//     - Store an ownerToken (not a Player* to avoid circular dependency)
//     - Support polymorphic behavior via onLand(Player*)
//
//   Ownership, pricing, and interaction logic are designed to work in
//   conjunction with GameController and Player.
//
// Related Modules:
//   - Player (used via pointer in onLand)
//   - GameController (executes ownership logic, triggers rent/payment)
//   - AcademicBuilding, Residence, Gym (inherit from Building)
//   - Square (base class)

export module Building;

import <string>;
import LandAction;
import Player;   // Used in onLand(Player*)
import Square;   // Building is-a Square

export class Building : public Square {
private:
    int price;                  // Purchase price of the property
    std::string ownerToken;     // Player token representing current owner ("BANK" if unowned)
    bool mortgaged = false;  // NEW: track mortgage status

public:
    // Constructs a building with name, board position, and price.
    Building(std::string name, int position, int price);  

    // Returns the name of the building (inherited from Square).
    std::string getName() const;

    // Returns the price required to purchase this property.
    int getPrice() const;

    // Returns the player token of the current owner.
    std::string getOwnerToken() const;

    // Sets the owner token of this building.
    void setOwnerToken(const std::string& token);

    // Called when a player lands on this square.
    // Will describe possible actions: buy, pay rent, or do nothing.
    LandAction onLand(Player* p) override;

    virtual int calculateRent(int context = 0) const = 0;

    bool isMortgaged() const;
    void setMortgaged(bool);
};
