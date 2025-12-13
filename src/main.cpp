#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include "constants.h"
#include "Text.h"
#include <vector>
#include <cmath>

SDL_Window*   window   = nullptr;
SDL_Renderer* renderer = nullptr;

int  playerScore = 0, computerScore = 0;
int  playerRoll = 1, computerRoll = 1;
bool rolling = false;
float rollTimer = 0.0f;
std::string message = Strings::ROLL_PROMPT.data();

void drawFilledCircle(SDL_Renderer* renderer, int cx, int cy, int radius) {
    for (int y = -radius; y <= radius; ++y) {
        int width = static_cast<int>(std::sqrt(radius * radius - y * y) + 0.5f);
        SDL_RenderLine(renderer, cx - width, cy + y, cx + width, cy + y);
    }
}

void drawDie(int value, int x, int y, int size = Dice::DIE_SIZE)
{
    const float fx = static_cast<float>(x);
    const float fy = static_cast<float>(y);
    const float fs = static_cast<float>(size);
    const float corner = fs * 0.18f;        // ~25px radius → perfect casino look
    const float dotR   = fs * 0.085f;
    const float off    = fs * 0.25f;

    // === 1. White face with perfect rounded corners (no green bleed!) ===
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Full white background
    SDL_FRect full{ fx, fy, fs, fs };
    SDL_RenderFillRect(renderer, &full);

    // === 2. Clean black border (inset by 1px so it never gets cut) ===
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_FRect border{ fx + 1, fy + 1, fs - 2, fs - 2 };
    SDL_RenderRect(renderer, &border);

    // === 3. Perfect black circular dots ===
    const float cx = fx + fs / 2.0f;
    const float cy = fy + fs / 2.0f;

    auto dot = [&](float dx, float dy) {
        drawFilledCircle(renderer, cx + dx, cy + dy, dotR);
    };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    if (value % 2 == 1)               dot( 0.0f,  0.0f);
    if (value >= 2) { dot(-off, -off); dot( off,  off); }
    if (value >= 4) { dot(-off,  off); dot( off, -off); }
    if (value == 6) { dot(-off, 0.0f); dot( off, 0.0f); }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_CreateWindowAndRenderer(Strings::GAME_NAME.data(), Screen::WIDTH, Screen::HEIGHT, 0, &window, &renderer);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    Text textRenderer(renderer);

    int fontBigId = -1;
    int fontMedId = -1;

    textRenderer.loadFont(Strings::FONT_PATH, 64, fontBigId);
    textRenderer.loadFont(Strings::FONT_PATH, 36, fontMedId);

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    bool quit = false;

    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT || 
                (e.type == SDL_EVENT_KEY_DOWN && 
                    e.key.key == SDLK_Q))
                quit = true;
            if (!rolling && (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
                             (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_SPACE))) {
                rolling = true;
                rollTimer = 0.5f;
                message = Strings::ROLLING_PROMPT.data();
            }
        }

        if (rolling) {
            rollTimer -= 1.0f / 60.0f;
            if (rollTimer > 0.0f) { // mid-roll
                playerRoll   = std::rand() % 6 + 1;
                computerRoll = std::rand() % 6 + 1;
            } else { // the roll is complete
                rolling = false;
                playerScore   += playerRoll;
                computerScore += computerRoll;
                message = (playerRoll > computerRoll) ? "You win this round!" :
                          (playerRoll < computerRoll) ? "Computer wins!" : "Tie!";
            }
        }

        SDL_SetRenderDrawColor(renderer, Color::FELT_R, Color::FELT_G, Color::FELT_B, 255);
        SDL_RenderClear(renderer);

        drawDie(playerRoll, 170, 200);
        drawDie(computerRoll, 490, 200);

        textRenderer.render(Strings::GAME_NAME.data(), fontBigId, Color::FELT_TEXT, 120.0f, 35.0f);
        textRenderer.renderCentered(message, fontBigId, Color::GOLD_TEXT,  Screen::HEIGHT-50.0);
        textRenderer.renderCentered("You: " + std::to_string(playerScore), fontMedId, Color::WHITE_TEXT, 390.0f);
        textRenderer.renderCentered("Computer: " + std::to_string(computerScore), fontMedId, Color::WHITE_TEXT, 440.0f);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}