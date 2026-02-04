#include "Game.h"

int main() {
    InitWindow(1000, 800, "Raylib Tic Tac Toe");
    SetTargetFPS(60);
    Game game;

    while (!WindowShouldClose()) {
        game.Update();
        game.Draw();
    }

    CloseWindow();
    return 0;
}
