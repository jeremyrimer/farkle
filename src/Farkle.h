#pragma once

#include <SDL3/SDL.h>
#include "constants.h"
#include "Text.h"
#include "Dice.h"
#include <string>
#include "Button.h"
#include <optional>

class Farkle {
public:
    explicit Farkle(SDL_Renderer* renderer);
    ~Farkle();

    Farkle(const Farkle&) = delete;
    Farkle& operator=(const Farkle&) = delete;

    struct DieLayout {
        static constexpr int SPACING  = 20;   // pixels between dice
        static constexpr int START_X  = 50;  // left margin

        static int getDieXPosition(int dieNumber) {
            return START_X + ((dieNumber-1) * (DiceConstants::DIE_SIZE + SPACING));
        }
    };

    void gameLoop();
private:
    SDL_Renderer* renderer{nullptr};

    // GAME STATE
    enum class TurnState {
        NotStarted,    // waiting for player to roll
        Rolled,        // rolled but can choose to hold
        Scoring,       // scoring dice, choosing to bank
        TurnOver       // turn ended
    };

    TurnState turnState{TurnState::NotStarted};

    bool playersTurn = true;
    int playerScore = 0; 
    int computerScore = 0;
    int handScore = 0;

    std::vector<int> diceRack;
    std::vector<bool> heldDice;

    struct DiceRect {
      SDL_FRect bounds;
      size_t diceRackIndex;
    };
    std::vector<DiceRect> diceRects;

    bool quit = false;
    bool rolling = false;
    bool banking = false;
    float rollTimer = 0.0f;

    // DISPLAY
    std::string message = StringConstants::INIT_MSG.data();
    Text text;
    Dice dice;
    int fontBigId;
    int fontMedId;
    int fontSmallId;

    static constexpr int STATUS_MESSAGE_HEIGHT = 210;
    static constexpr float GAME_NAME_X = 120.0f;
    static constexpr int GAME_NAME_Y = 35.0f;
    static constexpr int DIE_RENDER_Y = 75;
    static constexpr int YOUR_SCORE_X = 500;
    static constexpr int YOUR_SCORE_Y = 300;

    std::optional<Button> rollButton;
    std::optional<Button> bankButton;

    bool rollPressed{false};
    bool bankPressed{false};
    bool mouseLeftWasDownLastFrame{false};

    // METHODS
    void handleInput();
    void updateState();
    void render();
    void rollDice();
    void startTurn();
    void bankPoints();
    void nextPlayer();
};