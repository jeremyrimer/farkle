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
    int playerScore = 0; 
    int computerScore = 0;

    std::vector<int> diceRack;
    std::vector<bool> heldDice;

    bool quit = false;
    bool rolling = false;
    float rollTimer = 0.0f;

    // DISPLAY
    std::string message = StringConstants::ROLL_PROMPT.data();
    Text text;
    Dice dice;
    int fontBigId;
    int fontMedId;
    int fontSmallId;
    static constexpr int STATUS_MESSAGE_HEIGHT = 250;
    static constexpr float GAME_NAME_X = 120.0f;
    static constexpr int GAME_NAME_Y = 35.0f;
    static constexpr int DIE_RENDER_Y = 100;

    std::optional<Button> rollButton;
    std::optional<Button> bankButton;

    // METHODS
    void handleInput();
    void updateState();
    void render();
    void rollDice();
};