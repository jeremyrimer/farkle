#pragma once
#include <SDL3/SDL.h>
#include <string_view>

namespace ScreenConstants {
    constexpr int WIDTH  = 800;
    constexpr int HEIGHT = 650;
}

namespace StringConstants {
    constexpr std::string_view GAME_NAME = "FARKLE";
    constexpr std::string_view FONT_PATH = "assets/Roboto-Medium.ttf";
    constexpr std::string_view ROLL_PROMPT = "[Space] or Click to Roll";
    constexpr std::string_view ROLLING_PROMPT = "Rolling...";
}

namespace DiceConstants {
    constexpr int DIE_SIZE = 120;
}

namespace ColorConstants {
    constexpr SDL_Color WHITE_TEXT = {255, 255, 255, 255};
    constexpr SDL_Color GOLD_TEXT  = {255, 215,   0, 255};
    constexpr SDL_Color FELT_TEXT = {0, 175, 0, 255};
    constexpr int FELT_R = 34;
    constexpr int FELT_G = 139;
    constexpr int FELT_B = 34;
}