// src/Text.h
#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <string_view>
#include <vector>

class Text {
public:
    explicit Text(SDL_Renderer* renderer);
    ~Text();

    Text(const Text&) = delete;
    Text& operator=(const Text&) = delete;

    bool loadFont(std::string_view path, int ptsize, int& outFontId);
    void renderCentered(std::string_view text, int fontId, SDL_Color color, float y) const;
    void render(std::string_view text, int fontId, SDL_Color color, float x, float y) const;

private:
    SDL_Renderer* renderer_{nullptr};

    struct Font {
        TTF_Font* ttf{nullptr};
    };
    std::vector<Font> fonts_;  
};