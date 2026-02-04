#include "raylib.h"

// Constants for easy adjustments
const int SCREEN_SIZE = 600;
const int CELL_SIZE   = 200; // 600 / 3

int main() {
    // 1. Initialize Window
    InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Raylib Tic-Tac-Toe");
    SetTargetFPS(60);

    // 0 = Empty, 1 = X, 2 = O
    int  board[3][3] = {0};
    int  playerTurn  = 1; // Start with X
    bool gameOver    = false;

    Font genshinFont = LoadFontEx("resources/zh-cn.ttf", 96, 0, 0);
    SetTextureFilter(genshinFont.texture, TEXTURE_FILTER_BILINEAR);

    while (!WindowShouldClose()) {
        // --- 2. UPDATE LOGIC (Where the click handling happens) ---

        if (!gameOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();

            // HIGHLIGHT: Handling Individual Click Events
            // We convert the mouse pixel position (e.g., 450px)
            // into a grid index (e.g., 450 / 200 = index 2)
            int row = mousePos.y / CELL_SIZE;
            int col = mousePos.x / CELL_SIZE;

            // Check if the click is within bounds and the cell is empty
            if (row >= 0 && row < 3 && col >= 0 && col < 3) {
                if (board[row][col] == 0) {
                    board[row][col] = playerTurn;

                    // Switch turn: if 1, become 2; if 2, become 1
                    playerTurn = (playerTurn == 1) ? 2 : 1;
                }
            }
        }

        // Reset game on 'R'
        if (IsKeyPressed(KEY_R)) {
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    board[i][j] = 0;
            gameOver   = false;
            playerTurn = 1;
        }

        // --- 3. DRAWING LOGIC ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw the Grid Lines
        for (int i = 1; i < 3; i++) {
            DrawLine(i * CELL_SIZE, 20, i * CELL_SIZE, SCREEN_SIZE - 20, LIGHTGRAY); // Vertical
            DrawLine(20, i * CELL_SIZE, SCREEN_SIZE - 20, i * CELL_SIZE, LIGHTGRAY); // Horizontal
        }

        // Draw X and O based on board state
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                int centerX = col * CELL_SIZE + CELL_SIZE / 2;
                int centerY = row * CELL_SIZE + CELL_SIZE / 2;

                if (board[row][col] == 1) { // Draw X
                    DrawTextEx(genshinFont, "X", {(float) centerX - 40, (float) centerY - 50}, 100, 0, GRAY);
                } else if (board[row][col] == 2) { // Draw O

                    DrawTextEx(genshinFont, "O", {(float) centerX - 40, (float) centerY - 50}, 100, 0, GRAY);
                }
            }
        }

        if (gameOver)
            DrawText("GAME OVER! Press R to Restart", 50, SCREEN_SIZE / 2, 30, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
