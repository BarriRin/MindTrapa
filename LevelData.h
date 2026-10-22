#pragma once

// Time limits (in seconds) for earning stars on a level.
struct StarThresholds {
    float threeStarMaxTime; // <= this time earns 3 stars
    float twoStarMaxTime;   // <= this time earns 2 stars, otherwise 1 star
};

// Returns the star thresholds for the given level id (1-based). Out-of-range
// ids are clamped to the nearest valid level.
const StarThresholds& GetStarThresholds(int levelId);
