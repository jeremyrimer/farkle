#include "Farkle.h"
#include <SDL3/SDL.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include "constants.h"
#include "Text.h"
#include "Dice.h"
#include <vector>
#include <cmath>

Farkle::Farkle(SDL_Renderer* renderer) : 
  renderer_(renderer), 
  text(renderer), 
  dice(renderer) {
    text.loadFont(StringConstants::FONT_PATH, 64, fontBigId);
    text.loadFont(StringConstants::FONT_PATH, 36, fontMedId);
  }
Farkle::~Farkle() {}

void Farkle::gameLoop() {
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
                playerRoll   = dice.rollDie();
                computerRoll = dice.rollDie();
            } else { // the roll is complete
                rolling = false;
                playerScore   += playerRoll;
                computerScore += computerRoll;
                message = (playerRoll > computerRoll) ? "You win this round!" :
                            (playerRoll < computerRoll) ? "Computer wins!" : "Tie!";
            }
        }

        SDL_SetRenderDrawColor(renderer_, ColorConstants::FELT_R, ColorConstants::FELT_G, ColorConstants::FELT_B, 255);
        SDL_RenderClear(renderer_);

        dice.drawDie(playerRoll, 170, 200);
        dice.drawDie(computerRoll, 490, 200);

        text.render(StringConstants::GAME_NAME.data(), fontBigId, ColorConstants::FELT_TEXT, 120.0f, 35.0f);
        text.renderCentered(message, fontBigId, ColorConstants::GOLD_TEXT,  ScreenConstants::HEIGHT-50.0);
        text.renderCentered("You: " + std::to_string(playerScore), fontMedId, ColorConstants::WHITE_TEXT, 390.0f);
        text.renderCentered("Computer: " + std::to_string(computerScore), fontMedId, ColorConstants::WHITE_TEXT, 440.0f);

        SDL_RenderPresent(renderer_);
        SDL_Delay(16);
    }
}
