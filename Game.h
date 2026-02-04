#pragma once
#include "Core.h"
#include "raylib.h"
#include <string>
#include <vector>

enum GameState { MENU, PLAYING, GAME_OVER };

class Game {
  public:
    Game();
    void      Update();
    void      Draw();
    GameState currentState;

  private:
    void InitBoard(int size);
    void HandleInput();
    void DrawBoard();
    void DrawHistory();

    int                            boardSize;
    std::vector<std::vector<char>> board;
    char                           currentPlayer;
    Core                           logic;
    Core::BOARD_STATUS             status;
    std::vector<std::string>       history;
};
