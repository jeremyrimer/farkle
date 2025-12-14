// src/Dice.h
#pragma once

#include <SDL3/SDL.h>
#include "constants.h"

class Dice {
public:
    explicit Dice(SDL_Renderer* renderer);
    ~Dice();

    Dice(const Dice&) = delete;
    Dice& operator=(const Dice&) = delete;

    void drawDie(int value, int x, int y, int size = DiceConstants::DIE_SIZE) const;
    int rollDie();
private:
    SDL_Renderer* renderer{nullptr};
    
    void drawFilledCircle(int cx, int cy, int radius) const;
};