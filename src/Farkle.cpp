#include "Farkle.h"
#include <SDL3/SDL.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include "constants.h"
#include "Text.h"
#include "Dice.h"
#include <cmath>
#include <iostream>

Farkle::Farkle(SDL_Renderer* renderer) : 
  renderer(renderer), 
  text(renderer), 
  dice(renderer), 
  diceRects(6),
  diceRack(6, 1),
  heldDice(6, false) {
    text.loadFont(StringConstants::FONT_PATH, 64, fontBigId);
    text.loadFont(StringConstants::FONT_PATH, 36, fontMedId);
    text.loadFont(StringConstants::FONT_PATH, 24, fontSmallId);

    rollButton.emplace(
      renderer,
      text,
      fontSmallId,
      "Roll",
      "Roll",
      25,
      ScreenConstants::HEIGHT - 140,
      200,
      50
    );
    bankButton.emplace(
      renderer,
      text,
      fontSmallId,
      "Bank",
      "Bank",
      25,
      ScreenConstants::HEIGHT - 80,
      200,
      50
    );
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

        if (rollButton) rollButton->handleEvent(e);
        if (bankButton) bankButton->handleEvent(e);

        if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && 
              e.button.button == SDL_BUTTON_LEFT) {
            if (!mouseLeftWasDownLastFrame) {
                float mx = static_cast<float>(e.button.x);
                float my = static_cast<float>(e.button.y);

                for (size_t i = 0; i < diceRects.size(); ++i) {
                    const auto& dr = diceRects[i];
                    if (mx >= dr.bounds.x && mx <= dr.bounds.x + dr.bounds.w &&
                        my >= dr.bounds.y && my <= dr.bounds.y + dr.bounds.h) {
                        heldDice[dr.diceRackIndex] = !heldDice[dr.diceRackIndex];
                        break;
                    }
                }
                mouseLeftWasDownLastFrame = true;
            }
        } else mouseLeftWasDownLastFrame = false;

        if (!rolling && (rollButton ? rollButton->isClicked() : false)) {
            rolling = true;
            rollTimer = 0.5f;
        }

        if (!banking && (bankButton ? bankButton->isClicked() : false)) {
            banking = true;
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
            message = "Roll Score: " + std::to_string(handScore);
        }
    }

    if (banking) {
        bankPoints();
        banking = false;
    }

    rollButton->updateState();
    bankButton->updateState();
}

void Farkle::rollDice() {
    for (size_t i = 0; i < diceRack.size(); ++i) {
        if (!heldDice[i]) {
            diceRack[i] = std::rand() % 6 + 1;
        }
    }
    turnState = TurnState::Rolled;
}

void Farkle::render() {
    SDL_SetRenderDrawColor(renderer, ColorConstants::FELT_R, ColorConstants::FELT_G, ColorConstants::FELT_B, 255);
    SDL_RenderClear(renderer);

    for (size_t i = 0; i < diceRack.size(); ++i) {
        int x = DieLayout::getDieXPosition(static_cast<int>(i+1));

        // Highlight held dice
        SDL_FRect borderHighlight{ 
            static_cast<float>(x - 8), 
            static_cast<float>(DIE_RENDER_Y - 8),
            static_cast<float>(DiceConstants::DIE_SIZE + 16),
            static_cast<float>(DiceConstants::DIE_SIZE + 16) 
        };
        if (heldDice[i]) {
            SDL_SetRenderDrawColor(
                renderer, 
                ColorConstants::LIGHT_YELLOW.r, 
                ColorConstants::LIGHT_YELLOW.g,
                ColorConstants::LIGHT_YELLOW.b, 
                ColorConstants::LIGHT_YELLOW.a
            );
            SDL_RenderRect(renderer, &borderHighlight);
        }
        diceRects[i] = DiceRect{borderHighlight, i};
        dice.drawDie(diceRack[i], x, DIE_RENDER_Y, DiceConstants::DIE_SIZE);
    }

    text.render(StringConstants::GAME_NAME.data(), fontBigId, ColorConstants::FELT_GREEN, GAME_NAME_X, GAME_NAME_Y);
    text.renderCentered(message, fontMedId, ColorConstants::GOLD, STATUS_MESSAGE_HEIGHT);

    text.render(std::to_string(playerScore), fontMedId, ColorConstants::WHITE, 300, ScreenConstants::HEIGHT - 55);
    text.render("Opponent: " + std::to_string(computerScore), fontMedId, ColorConstants::WHITE, 620, ScreenConstants::HEIGHT - 55);

    if (rollButton) {
        rollButton->render();
    }

    if (bankButton) {
        bankButton->render();
    }
    SDL_RenderPresent(renderer);
}

void Farkle::startTurn() {
    turnState = TurnState::NotStarted;
    handScore = 0;
    std::fill(heldDice.begin(), heldDice.end(), false);
}

void Farkle::bankPoints() {
    if (playersTurn) {
      playerScore += handScore;
    }
    else {
      computerScore += handScore;
    }
    turnState = TurnState::TurnOver;
    nextPlayer();
    startTurn();
}

void Farkle::nextPlayer() {
    playersTurn = !playersTurn;
}