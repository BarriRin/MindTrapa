#pragma once

// Per-frame keyboard state cache. Call Update() exactly once per frame, then
// query IsHeld()/IsPressed() instead of hand-rolling a static debounce flag
// at every CheckHitKey() call site.
class Input {
public:
    void Update();

    bool IsHeld(int key) const;    // key is currently down
    bool IsPressed(int key) const; // key transitioned from up to down this frame

private:
    static constexpr int KEY_COUNT = 256;
    bool current[KEY_COUNT]  = {};
    bool previous[KEY_COUNT] = {};
};
