#pragma once

// Разрешение экрана — единственное место для правки при смене резолюции
// (раньше 1920/1080 было захардкожено отдельно в main.cpp, Camera.h и ~38 местах Menu.cpp)
static constexpr int SCREEN_W = 1920;
static constexpr int SCREEN_H = 1080;

// Per-level star thresholds: {3-star limit, 2-star limit} in seconds.
// Derived from best recorded playthroughs: 3★ = bracket above best, 2★ = 3★ × 1.5
extern const float STAR_THRESHOLDS[50][2];
