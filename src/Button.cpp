#include "Button.h"
#include "constants.h"

Button::Button(
    SDL_Renderer* renderer,
    Text& text,
    int fontId,
    const std::string& promptText,
    const std::string& activeText,
    float x,
    float y,
    float w,
    float h
)
    : renderer(renderer),
      text(text),
      fontId(fontId),
      promptText(promptText),
      activeText(activeText),
      bounds{ x, y, w, h }
{}

void Button::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_EVENT_MOUSE_MOTION) {
        float mx = (float)e.motion.x;
        float my = (float)e.motion.y;

        hovered =
            mx >= bounds.x &&
            mx <= bounds.x + bounds.w &&
            my >= bounds.y &&
            my <= bounds.y + bounds.h;
    }

    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
        e.button.button == SDL_BUTTON_LEFT &&
        hovered) {
        pressed = true;
    }

    if (e.type == SDL_EVENT_MOUSE_BUTTON_UP &&
        e.button.button == SDL_BUTTON_LEFT) {
        pressed = false;
    }
}

bool Button::isClicked() const {
    return hovered && pressed;
}

void Button::render() const {
    // Background
    if (pressed) {
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    } else if (hovered) {
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
    }
    SDL_RenderFillRect(renderer, &bounds);

    // Border
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderRect(renderer, &bounds);

    // Text
    const std::string& label = pressed ? activeText : promptText;
    
    text.render(label, fontId, ColorConstants::WHITE, bounds.x + bounds.w * 0.5f, bounds.y + bounds.h * 0.5f);
}

