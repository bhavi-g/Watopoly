// LandAction.cc (interface)
// Module: LandAction
// Description:
//   Defines the LandAction enum, used as the return type from
//   Square::onLand(Player*) in the Watopoly project.
//
//   This allows each square to communicate its intent to the GameController,
//   which then takes the appropriate gameplay action.
//
//   This design follows CS246 principles of:
//     - Inversion of Control (square does NOT act directly)
//     - Decoupling (no tight linkage to GameController)
//     - Clean MVC separation (model triggers signal, controller interprets)

export module LandAction;

/// Represents the result of a player landing on a square.
///
/// This is returned from `Square::onLand(Player*)` and interpreted by
/// the GameController. The controller will then trigger the appropriate
/// side effects (e.g., prompting to buy, paying rent, etc.).
///
/// This avoids direct calls to GameController from square logic,
/// preserving modularity and preventing circular dependencies.
export enum class LandAction {
    None,            ///< No gameplay effect (e.g., Goose Nesting, DC Tims Line)
    PromptPurchase,  ///< Player landed on an unowned property → controller prompts to buy
    PayRent,         ///< Player landed on an opponent’s property → controller deducts rent
    Owned,            ///< Player landed on their own property → no action needed

    // For Action Squares
    CollectOSAP,
    PayTuition,
    PayCoopFee,
    Teleport,
    MoneyEvent,
    GoToTims
};
