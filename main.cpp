#include "DxLib.h"
#include "Game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Game game;
    if (!game.Init()) return -1;
    game.Run();
    game.Shutdown();
    return 0;
}
