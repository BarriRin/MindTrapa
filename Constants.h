#pragma once

// Screen resolution — the single place to edit when changing resolution
// (used to be hardcoded separately in main.cpp, Camera.h, and ~38 spots in Menu.cpp)
static constexpr int SCREEN_W = 1920;
static constexpr int SCREEN_H = 1080;

// Per-level star thresholds: {3-star limit, 2-star limit} in seconds.
// Derived from best recorded playthroughs: 3★ = bracket above best, 2★ = 3★ × 1.5
extern const float STAR_THRESHOLDS[50][2];
