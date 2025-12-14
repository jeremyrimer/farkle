// src/Farkle.h
#pragma once

#include <SDL3/SDL.h>
#include "constants.h"
#include "Text.h"
#include "Dice.h"
#include <vector>
#include <cmath>
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
    bool quit = false;
    int playerScore = 0; 
    int computerScore = 0;
    int playerRoll = 1;
    int computerRoll = 1;
    bool rolling = false;
    float rollTimer = 0.0f;
    std::string message = StringConstants::ROLL_PROMPT.data();
    Text text;
    Dice dice;
    int fontBigId;
    int fontMedId;
};