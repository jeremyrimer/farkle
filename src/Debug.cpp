#include "Debug.h"
#include "constants.h"

Debug::Debug(
    SDL_Renderer* renderer,
    Text& text,
    int fontId
)
    : renderer(renderer),
      text(text),
      fontId(fontId)
{}

void Debug::handleEvent(const SDL_Event& e) {
    if (
        e.type == SDL_EVENT_KEY_DOWN && 
        e.key.key == SDLK_D) {
            if (!showWasDownLastFrame) {
            show = !show;
            showWasDownLastFrame = true;
        }
    } else showWasDownLastFrame = false;
}

void Debug::update(TurnState newTurnState, bool newPlayerTurn) {
    turnState = newTurnState;
    playerTurn = newPlayerTurn;
}

void Debug::render() {
    if (show) {
        text.render(playerTurn ? "Player's Turn" : "Computer's Turn", fontId, ColorConstants::WHITE, ScreenConstants::WIDTH - 100, 300);
        text.render("turnState: " + to_string(turnState), fontId, ColorConstants::WHITE, ScreenConstants::WIDTH - 100, 320);
    }
}