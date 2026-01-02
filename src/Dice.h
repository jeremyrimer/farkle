// src/Dice.h
#pragma once

#include <SDL3/SDL.h>
#include "constants.h"

struct DieLayout {
    static constexpr int SPACING  = 20;   // pixels between dice
    static constexpr int START_X  = 50;  // left margin

    static int getDieXPosition(int dieNumber) {
        return START_X + ((dieNumber-1) * (DiceConstants::DIE_SIZE + SPACING));
    }
};

class Dice {
public:
    explicit Dice(SDL_Renderer* renderer, int index);
    ~Dice();

    Dice(const Dice&) = delete;
    Dice& operator=(const Dice&) = delete;

    Dice(Dice&&) noexcept = default;
    Dice& operator=(Dice&&) noexcept = default;

    void render(int index, int size = DiceConstants::DIE_SIZE) const;
    void roll();
    bool isHeld() const;
    bool isPlayed() const;
    int getValue() const;
    void setHeld(bool newHeld);
    void toggleHeld();
    void setPlayed(bool newPlayed);
    void setBounds(SDL_FRect newBounds);
    SDL_FRect getBounds() const;
private:
    SDL_Renderer* renderer{nullptr};
    
    bool held = false;
    bool played = false;
    int value = 1;
    SDL_FRect bounds;
    
    void drawFilledCircle(int cx, int cy, int radius) const;
};