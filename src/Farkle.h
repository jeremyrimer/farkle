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

    void gameLoop();
private:
    SDL_Renderer* renderer_{nullptr};

    // GAME STATE
    int playerScore = 0; 
    int computerScore = 0;
    int playerRoll = 1;
    int computerRoll = 1;
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