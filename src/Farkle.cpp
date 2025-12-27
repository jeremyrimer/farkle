#include "Farkle.h"
#include <SDL3/SDL.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include "constants.h"
#include "Text.h"
#include "Dice.h"
#include "Debug.h"
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
    text.loadFont(StringConstants::FONT_PATH, 12, fontSmallId);

    debugModule.emplace(renderer, text, fontSmallId);

    rollButton.emplace(
      renderer,
      text,
      fontMedId,
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
      fontMedId,
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

        // mouse button press detected
        if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            if (playersTurn && 
                turnState == TurnState::Rolled &&
                e.button.button == SDL_BUTTON_LEFT) {

                // HOLD detection
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

            }
        }  else mouseLeftWasDownLastFrame = false;

        rollButton->handleEvent(e);
        if (!rolling && (rollButton ? rollButton->isClicked() : false)) {
            rolling = true;
            rollTimer = 0.5f;
        }

        bankButton->handleEvent(e);
        if (!banking && 
            playersTurn && turnState == TurnState::Rolled &&
            (bankButton ? bankButton->isClicked() : false)) {
            banking = true;
        }

        debugModule->handleEvent(e);
    }
}

void Farkle::updateState() {
    if (rolling) {
        turnState = TurnState::Rolling;
        message = StringConstants::ROLLING_PROMPT.data();
        rollTimer -= 1.0f / 60.0f;
        if (rollTimer > 0.0f) { // mid-roll
            rollDice();
        } else { // the roll is complete
            rolling = false;
            turnState = TurnState::Rolled;
        }
    }

    if (banking) {
        turnState = TurnState::Scoring;
        bankPoints();
        checkWin();
        startTurn();
    }

    debugModule->update(turnState, playersTurn);
    rollButton->updateState();
    bankButton->updateState();
}

void Farkle::rollDice() {
    for (size_t i = 0; i < diceRack.size(); ++i) {
        if (!heldDice[i]) {
            diceRack[i] = std::rand() % 6 + 1;
        }
    }
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
    SDL_Color playerColor = playersTurn ? ColorConstants::LIGHT_YELLOW : ColorConstants::WHITE;
    SDL_Color computerColor = !playersTurn ? ColorConstants::LIGHT_YELLOW : ColorConstants::WHITE;
    text.render(std::to_string(playerScore), fontMedId, playerColor, 300, ScreenConstants::HEIGHT - 55);
    text.render("Opponent: " + std::to_string(computerScore), fontMedId, computerColor, 620, ScreenConstants::HEIGHT - 55);

    if (rollButton) {
        rollButton->render();
    }

    if (bankButton) {
        bankButton->render();
    }

    if (debugModule) {
        debugModule->render();
    }

    SDL_RenderPresent(renderer);
}

void Farkle::startTurn() {
    nextPlayer();
    turnState = TurnState::FirstRoll;
    handScore = 0;
    std::fill(heldDice.begin(), heldDice.end(), false);
}

void Farkle::bankPoints() {
    turnState = TurnState::Scoring;
    if (playersTurn) {
      playerScore += handScore;
    }
    else {
      computerScore += handScore;
    }
    handScore = 0;
    banking = false;
}

void Farkle::nextPlayer() {
    playersTurn = !playersTurn;
}

void Farkle::tallyHandScore() {
    // calculate held dice and add it to handScore

}

void Farkle::checkWin() {
    if (lastRound) {
        if (playerScore >= 10000) {
            playerWins = true;
            message = "YOU WON!";
        }
        else if (computerScore >= 10000) {
            computerWins = true;
            message = "YOU LOSE";
        }
    }
}

bool Farkle::checkFarkle() {
    // return true if dice in farkle state, false if not
    return false;
}