#pragma once
#include <SDL3/SDL.h>
#include <string_view>
#include <array>
#include <string>

namespace ScreenConstants {
    constexpr int WIDTH  = 800;
    constexpr int HEIGHT = 650;
}

namespace StringConstants {
    constexpr std::string_view GAME_NAME = "FARKLE";
    constexpr std::string_view FONT_PATH = "assets/Roboto-Medium.ttf";
    constexpr std::string_view INIT_MSG = "Are you ready?";
    constexpr std::string_view ROLLING_PROMPT = "Rolling...";
    constexpr std::string_view ROLLED_PROMPT = "Go!";
    constexpr std::string_view COMPUTER_THINKING = "Thinking...";
}

namespace DiceConstants {
    constexpr int DIE_SIZE = 100;
    constexpr int DIE_RENDER_Y = 75;
    constexpr int NUM_DICE = 6;
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
    constexpr SDL_Color GRAY{ 100, 100, 100, 255 };

    constexpr int FELT_R = 34;
    constexpr int FELT_G = 139;
    constexpr int FELT_B = 34;
}

// GAME STATE
enum class TurnState {
    FirstRoll,     // waiting for player to roll 1st time
    Rolling,       // dice shuffling
    Rolled,        // rolled but can choose to hold
    Scoring,       // scoring dice, choosing to bank
    TurnOver,      // turn ended
    COUNT          // for the number of TurnStates
};

inline const std::array<const char*, static_cast<size_t>(TurnState::COUNT)> TurnStateStrings{
    "First Roll",
    "Rolling",
    "Rolled",
    "Scoring",
    "Turn Over"
};

inline std::string to_string(TurnState state) {
    size_t index = static_cast<size_t>(state);
    if (index >= TurnStateStrings.size()) return "Unknown";
    return TurnStateStrings[index];
}