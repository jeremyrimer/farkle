// main.cpp – Works perfectly with SDL3 + SDL3_ttf on macOS (November 2025)
// Zero errors, zero squiggles in VS Code
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdlib>
#include <ctime>
#include <string>

constexpr int WIDTH  = 800;
constexpr int HEIGHT = 650;

SDL_Window*   window   = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font*     fontBig  = nullptr;
TTF_Font*     fontMed  = nullptr;

int  playerScore = 0, computerScore = 0;
int  playerRoll = 1, computerRoll = 1;
bool rolling = false;
float rollTimer = 0.0f;
std::string message = "Click or press Space to roll!";

SDL_Texture* createText(const std::string& text, TTF_Font* font, SDL_Color fg) {
    if (text.empty()) return nullptr;
    // 0 = null-terminated string → safe and fast
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), 0, fg);
    if (!surface) {
        SDL_Log("TTF error: %s", SDL_GetError());
        return nullptr;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    return tex;
}

void drawDie(int value, int x, int y, int size = 140) {
    SDL_FRect face{ static_cast<float>(x), static_cast<float>(y), 
                    static_cast<float>(size), static_cast<float>(size) };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &face);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderRect(renderer, &face);

    const int c = x + size / 2;
    const int r = 14;
    const int o = size / 4;

    auto dot = [&](int dx, int dy) {
        SDL_FRect d{ static_cast<float>(c + dx - r), 
                     static_cast<float>(y + size/2 + dy - r), 
                     static_cast<float>(r*2), static_cast<float>(r*2) };
        SDL_RenderFillRect(renderer, &d);
    };

    if (value % 2 == 1) dot(0, 0);
    if (value >= 2) { dot(-o, -o); dot(o, o); }
    if (value >= 4) { dot(-o, o); dot(o, -o); }
    if (value == 6) { dot(-o, 0); dot(o, 0); }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_CreateWindowAndRenderer("Farkle", WIDTH, HEIGHT, 0, &window, &renderer);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    fontBig = TTF_OpenFont("assets/Roboto-Medium.ttf", 64);
    fontMed = TTF_OpenFont("assets/Roboto-Medium.ttf", 36);

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    bool quit = false;
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) quit = true;
            if (!rolling && (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
                             (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_SPACE))) {
                rolling = true;
                rollTimer = 0.5f;
                message = "Rolling...";
            }
        }

        if (rolling) {
            rollTimer -= 1.0f / 60.0f;
            if (rollTimer > 0.0f) {
                playerRoll   = std::rand() % 6 + 1;
                computerRoll = std::rand() % 6 + 1;
            } else {
                rolling = false;
                playerScore   += playerRoll;
                computerScore += computerRoll;
                message = (playerRoll > computerRoll) ? "You win this round!" :
                          (playerRoll < computerRoll) ? "Computer wins!" : "Tie!";
            }
        }

        SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255);
        SDL_RenderClear(renderer);

        drawDie(playerRoll,   170, 200);
        drawDie(computerRoll, 490, 200);

        SDL_Color white = {255, 255, 255, 255};
        SDL_Color gold  = {255, 215,   0, 255};

        auto renderCentered = [&](const std::string& txt, TTF_Font* f, SDL_Color col, float y) {
            SDL_Texture* tex = createText(txt, f, col);
            if (tex) {
                float w, h;
                SDL_GetTextureSize(tex, &w, &h);
                SDL_FRect dst{(WIDTH - w) / 2.0f, y, w, h};
                SDL_RenderTexture(renderer, tex, nullptr, &dst);
                SDL_DestroyTexture(tex);
            }
        };

        renderCentered(message,                              fontBig, gold,  50.0f);
        renderCentered("You: " + std::to_string(playerScore),   fontMed, white, 390.0f);
        renderCentered("Computer: " + std::to_string(computerScore), fontMed, white, 440.0f);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    TTF_CloseFont(fontBig);
    TTF_CloseFont(fontMed);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}