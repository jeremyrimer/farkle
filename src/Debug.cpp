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
    if (e.type == SDLK_F3) {
        show = !show;
    }
}

void Debug::update(TurnState newTurnState, bool newPlayerTurn) {
    turnState = newTurnState;
    playerTurn = newPlayerTurn;
}

void Debug::render() {
    if (show) {
        text.render("turnState: " + to_string(turnState), fontId, ColorConstants::WHITE, ScreenConstants::WIDTH - 100, 300);
    }
}