#include "DxLib.h"
#include "game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // 1) Устанавливаем желаемую резолюцию
    SetGraphMode(1024, 768, 32);
    // 2) Полноэкранный режим
    ChangeWindowMode(FALSE);
    if (DxLib_Init() == -1) return -1;
    SetDrawScreen(DX_SCREEN_BACK);

    Game game;
    while (ProcessMessage() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE))
    {
        game.update();
        game.draw();
        ScreenFlip();
    }

    DxLib_End();
    return 0;
}
