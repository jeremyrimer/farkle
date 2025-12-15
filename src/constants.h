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
    constexpr std::string_view ROLL_PROMPT = "Ready for Farkle?";
    constexpr std::string_view ROLLING_PROMPT = "Rolling...";
}

namespace DiceConstants {
    constexpr int DIE_SIZE = 100;
}

namespace ColorConstants {
    constexpr SDL_Color WHITE = {255, 255, 255, 255};
    constexpr SDL_Color BLACK = {0, 0, 0, 255};
    constexpr SDL_Color GOLD  = {255, 215, 0, 255};
    constexpr SDL_Color FELT_GREEN = {0, 175, 0, 255};
    constexpr SDL_Color LIGHT_YELLOW = {255, 255, 100, 255};
    constexpr SDL_Color BLUE{ 50, 100, 200, 255 }; 
    constexpr SDL_Color LIGHTER_BLUE{ 70, 130, 255, 255 }; 
    constexpr SDL_Color DARK_BLUE{ 30, 80, 180, 255 }; 

    constexpr int FELT_R = 34;
    constexpr int FELT_G = 139;
    constexpr int FELT_B = 34;
}