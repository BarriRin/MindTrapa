#include "Input.h"
#include "DxLib.h"

void Input::Update() {
    for (int i = 0; i < KEY_COUNT; i++) {
        previous[i] = current[i];
        current[i]  = CheckHitKey(i) != 0;
    }
}

bool Input::IsHeld(int key) const {
    return (key >= 0 && key < KEY_COUNT) && current[key];
}

bool Input::IsPressed(int key) const {
    return (key >= 0 && key < KEY_COUNT) && current[key] && !previous[key];
}
