#include "game.h"
#include "DxLib.h"

Game::Game()
    : floor(VGet(-50, -2, -50), VGet(50, 0, 50))
    , triggerCube(VGet(30, 0, 30), VGet(33, 3, 33))
    , nextLevel(false)
{
    camera.setTarget(&player.position);
}

void Game::update() {
    camera.update();
    player.update(camera);
    player.resolveCollision(floor);

    if (triggerCube.checkCollision(
        player.getAABBMin(), player.getAABBMax()
    )) {
        nextLevel = true;
    }
}

void Game::draw() {
    ClearDrawScreen();
    floor.draw(100, 100, 100);
    triggerCube.draw(0, 255, 255);
    player.draw();

    if (nextLevel) {
        // В Unicode-сборке нужен L-префикс:
        DrawFormatString(200, 30, GetColor(255, 255, 0),
            L"Level complete! (Trigger touched)");
    }
}
