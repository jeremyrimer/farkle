#pragma once
#include <SDL3/SDL.h>
#include <string_view>

constexpr int SCREEN_WIDTH  = 800;
constexpr int SCREEN_HEIGHT = 650;

constexpr std::string_view GAME_NAME = "Farkle";
constexpr std::string_view FONT_PATH = "assets/Roboto-Medium.ttf";

// Messages
constexpr std::string_view ROLL_PROMPT = "[Space] or Click to Roll";
constexpr std::string_view ROLLING_PROMPT = "Rolling...";

constexpr int DIE_SIZE = 140;

// Colors
SDL_Color WHITE_TEXT = {255, 255, 255, 255};
SDL_Color GOLD_TEXT  = {255, 215,   0, 255};
SDL_Color FELT_TEXT = {0, 175, 0, 255};
constexpr int FELT_R = 34;
constexpr int FELT_G = 139;
constexpr int FELT_B = 34;

