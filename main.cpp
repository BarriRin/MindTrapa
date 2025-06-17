#define NOMINMAX
#include "DxLib.h"
#include "game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    if (DxLib_Init() == -1) return -1;
    SetDrawScreen(DX_SCREEN_BACK);

    Game game;
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
        game.update();
        game.draw();
        ScreenFlip();
    }

    DxLib_End();
    return 0;
}
