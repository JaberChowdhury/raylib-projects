#include "Game.h"

Game::Game()
    : currentState(MENU)
    , boardSize(3)
    , currentPlayer('X') {}

void Game::InitBoard(int size) {
    boardSize = size;
    board.assign(size, std::vector<char>(size, ' '));
    status        = {false, ' '};
    currentPlayer = 'X';
    currentState  = PLAYING;
}

void Game::Update() {
    if (currentState == PLAYING)
        HandleInput();
    else if (currentState == GAME_OVER && IsKeyPressed(KEY_R))
        currentState = MENU;
}

void Game::HandleInput() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 m   = GetMousePosition();
        float   gap = 100, area = 600, cell = area / boardSize;

        if (m.x > gap && m.x < gap + area && m.y > gap && m.y < gap + area) {
            int col = (m.x - gap) / cell;
            int row = (m.y - gap) / cell;

            if (board[row][col] == ' ') {
                board[row][col] = currentPlayer;
                status          = logic.check_board(board, boardSize);
                if (status.isWin) {
                    history.push_back("Winner " + std::string(1, status.winner) + " (" + std::to_string(boardSize)
                                      + "x)");
                    currentState = GAME_OVER;
                } else {
                    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
                }
            }
        }
    }
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (currentState == MENU) {
        DrawText("SELECT SIZE", 300, 150, 40, BLACK);
        for (int i = 3; i <= 6; i++) {
            Rectangle btn = {300, (float) 120 + (i * 60), 200, 50};
            if (CheckCollisionPointRec(GetMousePosition(), btn)) {
                DrawRectangleRec(btn, LIGHTGRAY);
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    InitBoard(i);
            } else
                DrawRectangleRec(btn, GRAY);
            DrawText(TextFormat("%dx%d", i, i), 370, 135 + (i * 60), 20, WHITE);
        }
    } else {
        DrawBoard();
        DrawHistory();
        if (currentState == GAME_OVER) {
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(WHITE, 0.8f));
            DrawText(TextFormat("PLAYER %c WINS!", status.winner), 250, 300, 50, DARKGREEN);
            DrawText("Press 'R' to Menu", 320, 400, 20, DARKGRAY);
        }
    }
    EndDrawing();
}

void Game::DrawBoard() {
    float gap = 100, area = 600, cell = area / boardSize;
    for (int i = 0; i <= boardSize; i++) {
        DrawLineEx({gap + (i * cell), gap}, {gap + (i * cell), gap + area}, 2, BLACK);
        DrawLineEx({gap, gap + (i * cell)}, {gap + area, gap + (i * cell)}, 2, BLACK);
    }
    for (int r = 0; r < boardSize; r++) {
        for (int c = 0; c < boardSize; c++) {
            if (board[r][c] != ' ') {
                DrawText(TextFormat("%c", board[r][c]), gap + (c * cell) + cell / 4, gap + (r * cell), cell * 0.8,
                         (board[r][c] == 'X' ? RED : BLUE));
            }
        }
    }
}

void Game::DrawHistory() {
    DrawText("HISTORY", 750, 100, 20, BLACK);
    for (size_t i = 0; i < history.size(); i++)
        DrawText(history[i].c_str(), 750, 130 + (i * 25), 16, GRAY);
}
