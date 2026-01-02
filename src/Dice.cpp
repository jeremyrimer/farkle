#include "Dice.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "constants.h"
#include <cmath>
#include <iostream>

Dice::Dice(SDL_Renderer* renderer, int index) : renderer(renderer) {
    int x = DieLayout::getDieXPosition(index + 1);
    int y = DiceConstants::DIE_RENDER_Y;
    SDL_FRect borderHighlight{ 
        static_cast<float>(x - 8), 
        static_cast<float>(DiceConstants::DIE_RENDER_Y - 8),
        static_cast<float>(DiceConstants::DIE_SIZE + 16),
        static_cast<float>(DiceConstants::DIE_SIZE + 16) 
    };
    setBounds(borderHighlight);
}
Dice::~Dice() {}

void Dice::drawFilledCircle(int cx, int cy, int radius) const {
    for (int y = -radius; y <= radius; ++y) {
        int width = static_cast<int>(std::sqrt(radius * radius - y * y) + 0.5f);
        SDL_RenderLine(renderer, cx - width, cy + y, cx + width, cy + y);
    }
}

void Dice::render(int index, int size) const {
    int x = DieLayout::getDieXPosition(index + 1);
    int y = DiceConstants::DIE_RENDER_Y;
    const float fx = static_cast<float>(x);
    const float fy = static_cast<float>(y);
    const float fs = static_cast<float>(size);
    const float corner = fs * 0.18f;       
    const float dotR   = fs * 0.085f;
    const float off    = fs * 0.25f;

    // === 1. White face with perfect rounded corners (no green bleed!) ===
    SDL_SetRenderDrawColor(
        renderer, 
        ColorConstants::WHITE.r,
        ColorConstants::WHITE.g,
        ColorConstants::WHITE.b, 
        ColorConstants::WHITE.a);

    // Full white background
    SDL_FRect full{ fx, fy, fs, fs };
    SDL_RenderFillRect(renderer, &full);

    // === 2. Clean black border (inset by 1px so it never gets cut) ===
    SDL_SetRenderDrawColor(
        renderer, 
        ColorConstants::BLACK.r, 
        ColorConstants::BLACK.g, 
        ColorConstants::BLACK.b, 
        ColorConstants::BLACK.a
    );
    SDL_FRect border{ fx + 1, fy + 1, fs - 2, fs - 2 };
    SDL_RenderRect(renderer, &border);

    // === 3. Perfect black circular dots ===
    const float cx = fx + fs / 2.0f;
    const float cy = fy + fs / 2.0f;

    auto dot = [&](float dx, float dy) {
        drawFilledCircle(cx + dx, cy + dy, dotR);
    };

    SDL_SetRenderDrawColor(
        renderer, 
        ColorConstants::BLACK.r, 
        ColorConstants::BLACK.g, 
        ColorConstants::BLACK.b, 
        ColorConstants::BLACK.a
    );
    
    if (value % 2 == 1) dot( 0.0f,  0.0f);
    if (value >= 2) { dot(-off, -off); dot( off,  off); }
    if (value >= 4) { dot(-off,  off); dot( off, -off); }
    if (value == 6) { dot(-off, 0.0f); dot( off, 0.0f); }
    if (isHeld()) {
        int x = DieLayout::getDieXPosition(index+1);
        SDL_SetRenderDrawColor(
            renderer, 
            ColorConstants::LIGHT_YELLOW.r, 
            ColorConstants::LIGHT_YELLOW.g,
            ColorConstants::LIGHT_YELLOW.b, 
            ColorConstants::LIGHT_YELLOW.a
        );
        SDL_FRect b = getBounds();
        SDL_RenderRect(renderer, &b);
    }
}

void Dice::roll() {
    value = std::rand() % 6 + 1;
}

bool Dice::isHeld() const {
    return held;
}

bool Dice::isPlayed() const {
    return played;
}

int Dice::getValue() const {
    return value;
}

void Dice::setPlayed(bool newPlayed) {
    played = newPlayed;
}

void Dice::setHeld(bool newHeld) {
    held = newHeld;
}

SDL_FRect Dice::getBounds() const {
    return bounds;
}

void Dice::setBounds(SDL_FRect newBounds) {
    bounds = newBounds;
}

void Dice::toggleHeld() {
    held = !held;
}