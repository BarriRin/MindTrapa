#pragma once

// Game states
enum class GameState {
    PROFILE_SELECT,        // Select/create a profile (startup screen or profile switch)
    PROFILE_CREATE,        // Enter a nickname while creating a profile
    MAIN_MENU,
    LEVEL_SELECT_BLOCKS,   // Block selection (10 blocks)
    LEVEL_SELECT_LEVELS,   // Level selection within a block (10 levels)
    SETTINGS,              // Settings (mouse, music, sound)
    MUSIC_SELECT,          // Music track selection
    PLAYING,
    PAUSED,
    DYING,
    LEVEL_CELEBRATING,
    LEVEL_RESULT
};
