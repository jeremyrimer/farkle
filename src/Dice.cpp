#include "Dice.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "constants.h"
#include <cmath>

Dice::Dice(SDL_Renderer* renderer) : renderer(renderer) {}
Dice::~Dice() {}

void Dice::drawFilledCircle(int cx, int cy, int radius) const {
    for (int y = -radius; y <= radius; ++y) {
        int width = static_cast<int>(std::sqrt(radius * radius - y * y) + 0.5f);
        SDL_RenderLine(renderer, cx - width, cy + y, cx + width, cy + y);
    }
}

void Dice::drawDie(int value, int x, int y, int size) const
{
    const float fx = static_cast<float>(x);
    const float fy = static_cast<float>(y);
    const float fs = static_cast<float>(size);
    const float corner = fs * 0.18f;        // ~25px radius → perfect casino look
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
}

int Dice::rollDie() {
    return std::rand() % 6 + 1;
}
