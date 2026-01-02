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
  text(renderer){
    diceRack.reserve(DiceConstants::NUM_DICE);
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
    initDice();
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

                    for (size_t i = 0; i < diceRack.size(); ++i) {
                        const auto& bounds = diceRack[i].getBounds();
                        if (mx >= bounds.x && mx <= bounds.x + bounds.w &&
                            my >= bounds.y && my <= bounds.y + bounds.h) {
                            diceRack[i].toggleHeld();
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
            if (playersTurn) {
                message = StringConstants::ROLLED_PROMPT.data();
            } else {
                message = StringConstants::COMPUTER_THINKING.data();
            }
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
        if (!diceRack[i].isHeld()) {
            diceRack[i].roll();
        }
    }
}

void Farkle::render() {
    SDL_SetRenderDrawColor(renderer, ColorConstants::FELT_R, ColorConstants::FELT_G, ColorConstants::FELT_B, 255);
    SDL_RenderClear(renderer);

    for (size_t i = 0; i < diceRack.size(); ++i) {
        diceRack[i].render(i, DiceConstants::DIE_SIZE);
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

void Farkle::initDice() {
    for (size_t i = 0; i < DiceConstants::NUM_DICE; ++i) {
        diceRack.emplace_back(renderer, i);
    }
}