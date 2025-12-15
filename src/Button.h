#pragma once

#include <SDL3/SDL.h>
#include <string>
#include "Text.h"

class Button {
public:
    Button(
        SDL_Renderer* renderer,
        Text& text,
        int fontId,
        const std::string& promptText,
        const std::string& activeText,
        float x,
        float y,
        float w = 180.0f,
        float h = 50.0f
    );

    void render() const;
    void handleEvent(const SDL_Event& e);
    bool isClicked() const;

private:
    SDL_Renderer* renderer{nullptr};
    Text& text;
    int fontId;
    std::string promptText;
    std::string activeText;

    SDL_FRect bounds;
    bool hovered{false};
    bool pressed{false};
};
