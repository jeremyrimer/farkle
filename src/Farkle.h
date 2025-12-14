#pragma once

#include <SDL3/SDL.h>
#include "constants.h"
#include "Text.h"
#include "Dice.h"
#include <string>

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
    SDL_Renderer* renderer_{nullptr};

    // GAME STATE
    int playerScore = 0; 
    int computerScore = 0;

    int dieValue1 = 1;
    int dieValue2 = 1;
    int dieValue3 = 1;
    int dieValue4 = 1;
    int dieValue5 = 1;
    int dieValue6 = 1;

    bool quit = false;
    bool rolling = false;
    float rollTimer = 0.0f;

    // DISPLAY
    std::string message = StringConstants::ROLL_PROMPT.data();
    Text text;
    Dice dice;
    int fontBigId;
    int fontMedId;

    void handleInput();
    void updateState();
    void render();
};