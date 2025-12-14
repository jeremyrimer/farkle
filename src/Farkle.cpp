#include "Farkle.h"
#include <SDL3/SDL.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include "constants.h"
#include "Text.h"
#include "Dice.h"
#include <cmath>

Farkle::Farkle(SDL_Renderer* renderer) : 
  renderer_(renderer), 
  text(renderer), 
  dice(renderer), 
  diceRack(6, 1),
  heldDice(6, false) {
    text.loadFont(StringConstants::FONT_PATH, 64, fontBigId);
    text.loadFont(StringConstants::FONT_PATH, 36, fontMedId);
  }
Farkle::~Farkle() {}

void Farkle::gameLoop() {
    while (!quit) {
        handleInput();
        updateState();
        render();
        SDL_Delay(16);
    }
}

void Farkle::handleInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT || 
            (e.type == SDL_EVENT_KEY_DOWN && 
                (e.key.key == SDLK_Q | e.key.key == SDLK_ESCAPE)))
            quit = true;
        if (!rolling && (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
                            (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_SPACE))) {
            rolling = true;
            rollTimer = 0.5f;
        }
    }
}

void Farkle::updateState() {
    if (rolling) {
        message = StringConstants::ROLLING_PROMPT.data();
        rollTimer -= 1.0f / 60.0f;
        if (rollTimer > 0.0f) { // mid-roll
            rollDice();
        } else { // the roll is complete
            rolling = false;
            message = "roll complete";
        }
    }
}

void Farkle::rollDice() {
    for (size_t i = 0; i < diceRack.size(); ++i) {
        if (!heldDice[i]) {
            diceRack[i] = std::rand() % 6 + 1;
        }
    }
}

void Farkle::render() {
    SDL_SetRenderDrawColor(renderer_, ColorConstants::FELT_R, ColorConstants::FELT_G, ColorConstants::FELT_B, 255);
    SDL_RenderClear(renderer_);
    int dieRenderY = 100;

    for (size_t i = 0; i < diceRack.size(); ++i) {
        int x = DieLayout::getDieXPosition(static_cast<int>(i+1));

        // Highlight held dice
        if (heldDice[i]) {
            SDL_SetRenderDrawColor(renderer_, 255, 255, 100, 255);  // light yellow
            SDL_FRect border{ static_cast<float>(x - 8), static_cast<float>(dieRenderY - 8),
                              static_cast<float>(DiceConstants::DIE_SIZE + 16),
                              static_cast<float>(DiceConstants::DIE_SIZE + 16) };
            SDL_RenderRect(renderer_, &border);
        }

        dice.drawDie(diceRack[i], x, dieRenderY, DiceConstants::DIE_SIZE);
    }

    text.render(StringConstants::GAME_NAME.data(), fontBigId, ColorConstants::FELT_TEXT, 120.0f, 35.0f);
    text.renderCentered(message, fontBigId, ColorConstants::GOLD_TEXT,  ScreenConstants::HEIGHT-50.0);
    // text.renderCentered("You: " + std::to_string(playerScore), fontMedId, ColorConstants::WHITE_TEXT, 390.0f);
    // text.renderCentered("Computer: " + std::to_string(computerScore), fontMedId, ColorConstants::WHITE_TEXT, 440.0f);

    SDL_RenderPresent(renderer_);
}
