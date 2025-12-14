#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include "constants.h"
#include "Text.h"
#include "Dice.h"
#include <vector>
#include <cmath>

SDL_Window*   window   = nullptr;
SDL_Renderer* renderer = nullptr;

int  playerScore = 0, computerScore = 0;
int  playerRoll = 1, computerRoll = 1;
bool rolling = false;
float rollTimer = 0.0f;
std::string message = StringConstants::ROLL_PROMPT.data();

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_CreateWindowAndRenderer(StringConstants::GAME_NAME.data(), ScreenConstants::WIDTH, ScreenConstants::HEIGHT, 0, &window, &renderer);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    Text textRenderer(renderer);
    Dice diceRenderer(renderer);

    int fontBigId = -1;
    int fontMedId = -1;

    textRenderer.loadFont(StringConstants::FONT_PATH, 64, fontBigId);
    textRenderer.loadFont(StringConstants::FONT_PATH, 36, fontMedId);

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
                message = StringConstants::ROLLING_PROMPT.data();
            }
        }

        if (rolling) {
            rollTimer -= 1.0f / 60.0f;
            if (rollTimer > 0.0f) { // mid-roll
                playerRoll   = diceRenderer.rollDie();
                computerRoll = diceRenderer.rollDie();
            } else { // the roll is complete
                rolling = false;
                playerScore   += playerRoll;
                computerScore += computerRoll;
                message = (playerRoll > computerRoll) ? "You win this round!" :
                          (playerRoll < computerRoll) ? "Computer wins!" : "Tie!";
            }
        }

        SDL_SetRenderDrawColor(renderer, ColorConstants::FELT_R, ColorConstants::FELT_G, ColorConstants::FELT_B, 255);
        SDL_RenderClear(renderer);

        diceRenderer.drawDie(playerRoll, 170, 200);
        diceRenderer.drawDie(computerRoll, 490, 200);

        textRenderer.render(StringConstants::GAME_NAME.data(), fontBigId, ColorConstants::FELT_TEXT, 120.0f, 35.0f);
        textRenderer.renderCentered(message, fontBigId, ColorConstants::GOLD_TEXT,  ScreenConstants::HEIGHT-50.0);
        textRenderer.renderCentered("You: " + std::to_string(playerScore), fontMedId, ColorConstants::WHITE_TEXT, 390.0f);
        textRenderer.renderCentered("Computer: " + std::to_string(computerScore), fontMedId, ColorConstants::WHITE_TEXT, 440.0f);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}