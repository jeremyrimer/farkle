#pragma once

#include <SDL3/SDL.h>
#include <string>
#include "constants.h"
#include "Text.h"

class Debug {
public:
    Debug(
        SDL_Renderer* renderer,
        Text& text,
        int fontId
    );

    void handleEvent(const SDL_Event& e);
    void update(TurnState newTurnState, bool newPlayerTurn);
    void render();

private:
    SDL_Renderer* renderer{nullptr};
    Text& text;
    int fontId{0};

    bool show{false};
    bool showWasDownLastFrame{false};
    TurnState turnState{TurnState::FirstRoll};
    bool playerTurn{true};
};
