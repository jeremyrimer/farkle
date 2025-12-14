#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "constants.h"
#include "Farkle.h"

SDL_Window*   window   = nullptr;
SDL_Renderer* renderer = nullptr;

int  playerScore = 0, computerScore = 0;
int  playerRoll = 1, computerRoll = 1;
bool rolling = false;
float rollTimer = 0.0f;
std::string message = StringConstants::ROLL_PROMPT.data();

int main() {
    // INIT SDL
    SDL_Init(SDL_INIT_VIDEO);
    // INIT SDL TTF
    TTF_Init();

    // WINDOW
    SDL_CreateWindowAndRenderer(StringConstants::GAME_NAME.data(), ScreenConstants::WIDTH, ScreenConstants::HEIGHT, 0, &window, &renderer);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    // RANDOM
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // GAME
    Farkle farkle = Farkle(renderer);
    farkle.gameLoop();

    // SHUTDOWN; SDL CLEANUP
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}