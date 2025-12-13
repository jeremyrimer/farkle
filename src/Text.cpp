#include "Text.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include "constants.h"

Text::Text(SDL_Renderer* renderer) : renderer_(renderer) {
    if (!TTF_Init()) {
        std::cerr << "TTF_Init failed: " << SDL_GetError() << std::endl;
    }
}

Text::~Text() {
    for (auto& font : fonts_) {
        if (font.ttf) TTF_CloseFont(font.ttf);
    }
    TTF_Quit();
}

bool Text::loadFont(std::string_view path, int ptsize, int& outFontId) {
    TTF_Font* font = TTF_OpenFont(path.data(), static_cast<float>(ptsize));
    if (!font) {
        std::cerr << "Failed to load font " << path << ": " << SDL_GetError() << std::endl;
        return false;
    }

    outFontId = static_cast<int>(fonts_.size());
    fonts_.push_back({font});
    return true;
}

void Text::renderCentered(std::string_view text, int fontId, SDL_Color color, float y) const {
    if (text.empty() || fontId < 0 || fontId >= static_cast<int>(fonts_.size())) return;
    render(text, fontId, color, ScreenConstants::WIDTH / 2.0f, y);  // center X
}

void Text::render(std::string_view text, int fontId, SDL_Color color, float x, float y) const {
    if (text.empty() || fontId < 0 || fontId >= static_cast<int>(fonts_.size())) return;
    if (!fonts_[fontId].ttf) return;

    SDL_Surface* surface = TTF_RenderText_Blended(fonts_[fontId].ttf, text.data(), 0, color);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_DestroySurface(surface);
    if (!texture) return;

    float w, h;
    SDL_GetTextureSize(texture, &w, &h);

    SDL_FRect dst{x - w/2.0f, y - h/2.0f, w, h};  
    SDL_RenderTexture(renderer_, texture, nullptr, &dst);
    SDL_DestroyTexture(texture);
}