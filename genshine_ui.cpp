#include "raylib.h"
#include <vector>

// ----------------------------------------------------------------------------------
// DATA STRUCTURES
// ----------------------------------------------------------------------------------
enum Rarity { THREE_STAR, FOUR_STAR, FIVE_STAR };

struct Artifact {
    Rarity rarity;
    int    level;
    bool   isNew;
    bool   isLocked;
    bool   isEquipped;
};

// ----------------------------------------------------------------------------------
// COLOR PALETTE
// ----------------------------------------------------------------------------------
Color COL_BG_DARK      = {20, 24, 34, 255};
Color COL_PANEL_CREAM  = {236, 233, 216, 255};
Color COL_PANEL_HEADER = {188, 105, 50, 255};
Color COL_SELECTION    = {255, 255, 255, 200};

Color COL_R5_TOP = {201, 143, 98, 255};
Color COL_R5_BOT = {138, 95, 60, 255};
Color COL_R4_TOP = {162, 126, 196, 255};
Color COL_R4_BOT = {108, 85, 133, 255};
Color COL_R3_TOP = {101, 149, 196, 255};
Color COL_R3_BOT = {70, 100, 130, 255};

Color COL_TEXT_DARK   = {73, 83, 102, 255};
Color COL_TEXT_GREEN  = {101, 177, 86, 255};
Color COL_TEXT_GREY   = {120, 120, 120, 255};
Color COL_STAR_YELLOW = {255, 204, 50, 255};

// ----------------------------------------------------------------------------------
// HELPER FUNCTIONS
// ----------------------------------------------------------------------------------
void DrawStars(int count, int x, int y, int size) {
    for (int i = 0; i < count; i++) {
        DrawPoly((Vector2) {(float) x + (i * size * 1.3f), (float) y}, 5, size / 2.0f, 0, COL_STAR_YELLOW);
    }
}

int main(void) {
    // ----------------------------------------------------------------------------------
    // INITIALIZATION (1440p)
    // ----------------------------------------------------------------------------------
    const int screenWidth  = 2560;
    const int screenHeight = 1440;

    // Enable MSAA for smoother scaling on high res
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(screenWidth, screenHeight, "Genshin UI 1440p - Raylib");

    // Load Font - Larger size for high res
    Font genshinFont = LoadFontEx("resources/zh-cn.ttf", 96, 0, 0);
    SetTextureFilter(genshinFont.texture, TEXTURE_FILTER_BILINEAR);

    // Generate Dummy Data (50 Items to fill the larger screen)
    std::vector<Artifact> inventory;
    for (int i = 0; i < 50; i++) {
        Artifact a;
        int      r = GetRandomValue(0, 100);
        if (r > 70)
            a.rarity = FIVE_STAR;
        else if (r > 30)
            a.rarity = FOUR_STAR;
        else
            a.rarity = THREE_STAR;

        a.level      = GetRandomValue(0, 20);
        a.isLocked   = (GetRandomValue(0, 1) == 1);
        a.isNew      = (GetRandomValue(0, 5) == 0);
        a.isEquipped = (GetRandomValue(0, 3) == 0);
        inventory.push_back(a);
    }

    // Force specific item stats for demo
    inventory[0].rarity     = FIVE_STAR;
    inventory[0].level      = 15;
    inventory[0].isEquipped = true;

    SetTargetFPS(165); // Matching your 165Hz display

    // --- SCALED LAYOUT CONSTANTS (1.35x Scale) ---
    int startX = 140;
    int startY = 160;

    // Wider cards for 1440p
    int cardW = 180;
    int cardH = 220;
    int gap   = 24;

    // Calculate columns based on available width minus panel
    // Panel takes ~600px, leaving ~1900px
    int cols = 8;

    int selectedIndex = 0;

    // Right Panel Dimensions
    int panelW = 600;
    int panelH = 1100;
    // Dynamic X position (Anchored Right)
    int panelX = screenWidth - panelW - 100;
    int panelY = 160;

    // ----------------------------------------------------------------------------------
    // MAIN LOOP
    // ----------------------------------------------------------------------------------
    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();

        BeginDrawing();
        ClearBackground(COL_BG_DARK);

        // --- 1. TOP BAR ---
        DrawRectangle(0, 0, screenWidth, 110, Fade(BLACK, 0.2f));
        DrawTextEx(genshinFont, "Artifacts", (Vector2) {140, 35}, 48, 1, WHITE);

        // Filter Icons
        for (int i = 0; i < 5; i++)
            DrawRectangleLines(400 + (i * 80), 30, 55, 55, LIGHTGRAY);

        // --- 2. GRID RENDER ---
        for (int i = 0; i < inventory.size(); i++) {
            int col = i % cols;
            int row = i / cols;

            int       x        = startX + col * (cardW + gap);
            int       y        = startY + row * (cardH + gap);
            Rectangle cardRect = {(float) x, (float) y, (float) cardW, (float) cardH};

            bool isHover = CheckCollisionPointRec(mousePos, cardRect);
            if (isHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                selectedIndex = i;

            // Rarity Colors
            Color topColor = COL_R3_TOP;
            Color botColor = COL_R3_BOT;
            int   stars    = 3;
            if (inventory[i].rarity == FOUR_STAR) {
                topColor = COL_R4_TOP;
                botColor = COL_R4_BOT;
                stars    = 4;
            }
            if (inventory[i].rarity == FIVE_STAR) {
                topColor = COL_R5_TOP;
                botColor = COL_R5_BOT;
                stars    = 5;
            }

            DrawRectangleGradientV(x, y, cardW, cardH, topColor, botColor);

            // Icon Placeholder
            DrawCircle(x + cardW / 2, y + cardH / 2 - 15, 55, Fade(WHITE, 0.2f));

            // Stars
            DrawStars(stars, x + 20, y + cardH - 45, 16);

            // Level Badge
            if (inventory[i].level > 0) {
                DrawRectangle(x + 8, y + cardH - 32, 60, 26, Fade(BLACK, 0.5f));
                DrawTextEx(genshinFont, TextFormat("+%d", inventory[i].level),
                           (Vector2) {(float) x + 12, (float) y + cardH - 32}, 22, 0, WHITE);
            }

            // Lock
            if (inventory[i].isLocked) {
                DrawCircle(x + cardW - 20, y + 20, 8, Fade(ORANGE, 0.8f));
                DrawCircleLines(x + cardW - 20, y + 20, 8, WHITE);
            }

            // Equipped Face
            if (inventory[i].isEquipped) {
                DrawCircle(x + cardW - 25, y + cardH - 25, 22, WHITE);
                DrawCircle(x + cardW - 25, y + cardH - 25, 20, LIGHTGRAY);
            }

            // Selection
            if (i == selectedIndex)
                DrawRectangleLinesEx(cardRect, 4, WHITE);
            else if (isHover)
                DrawRectangleLinesEx(cardRect, 3, Fade(WHITE, 0.5f));
        }

        // --- 3. RIGHT PANEL (SCALED UP) ---
        DrawRectangle(panelX, panelY, panelW, panelH, COL_PANEL_CREAM);

        // Header
        DrawRectangle(panelX, panelY, panelW, 120, COL_PANEL_HEADER);
        DrawTextEx(genshinFont, "Maiden's Distant Love", (Vector2) {(float) panelX + 30, (float) panelY + 25}, 40, 0,
                   WHITE);
        DrawTextEx(genshinFont, "Flower of Life", (Vector2) {(float) panelX + 30, (float) panelY + 75}, 28, 0,
                   Fade(WHITE, 0.9f));

        // Big Icon
        DrawCircleGradient(panelX + 450, panelY + 200, 80, Fade(COL_R5_TOP, 0.5f), Fade(WHITE, 0.0f));

        // Stats Box
        int cursorY = panelY + 160;

        DrawRectangle(panelX + 30, cursorY, 260, 80, Fade(GRAY, 0.1f));
        DrawTextEx(genshinFont, "HP", (Vector2) {(float) panelX + 45, (float) cursorY + 15}, 26, 0, COL_TEXT_GREY);
        DrawTextEx(genshinFont, "3,764", (Vector2) {(float) panelX + 45, (float) cursorY + 40}, 52, 0, BLACK);

        DrawStars(5, panelX + 45, cursorY + 100, 20);

        // Level Badge Big
        DrawRectangle(panelX + 45, cursorY + 140, 80, 40, BLACK);
        DrawTextEx(genshinFont, "+15", (Vector2) {(float) panelX + 55, (float) cursorY + 145}, 30, 0, WHITE);

        // Substats
        cursorY += 210;
        DrawRectangle(panelX + 30, cursorY, 540, 2, Fade(GRAY, 0.3f));
        cursorY += 30;

        auto DrawStat = [&](const char* txt) {
            DrawTextEx(genshinFont, txt, (Vector2) {(float) panelX + 45, (float) cursorY}, 30, 0, COL_TEXT_DARK);
            cursorY += 45;
        };

        DrawStat(". DEF+5.1%");
        DrawStat(". HP+9.3%");
        DrawStat(". DEF+21");
        DrawStat(". CRIT Rate+6.6%");

        // Set Bonus
        cursorY += 30;
        DrawTextEx(genshinFont, "Maiden Beloved:", (Vector2) {(float) panelX + 45, (float) cursorY}, 30, 0,
                   COL_TEXT_GREEN);
        cursorY += 40;

        Color descColor = {100, 100, 100, 255};
        int   descSize  = 24;
        int   lineSpace = 28;

        DrawTextEx(genshinFont, "2-Piece Set: Character Healing", (Vector2) {(float) panelX + 45, (float) cursorY},
                   descSize, 0, descColor);
        cursorY += lineSpace;
        DrawTextEx(genshinFont, "Effectiveness +15%", (Vector2) {(float) panelX + 70, (float) cursorY}, descSize, 0,
                   descColor);
        cursorY += 40;

        DrawTextEx(genshinFont, "4-Piece Set: Using an Elemental", (Vector2) {(float) panelX + 45, (float) cursorY},
                   descSize, 0, descColor);
        cursorY += lineSpace;
        DrawTextEx(genshinFont, "Skill or Burst increases healing", (Vector2) {(float) panelX + 45, (float) cursorY},
                   descSize, 0, descColor);
        cursorY += lineSpace;
        DrawTextEx(genshinFont, "received by all party members", (Vector2) {(float) panelX + 45, (float) cursorY},
                   descSize, 0, descColor);
        cursorY += lineSpace;
        DrawTextEx(genshinFont, "by 20% for 10s.", (Vector2) {(float) panelX + 45, (float) cursorY}, descSize, 0,
                   descColor);
        cursorY += 50;

        // Lore
        DrawRectangle(panelX + 30, cursorY, 540, 140, Fade(GRAY, 0.1f));
        DrawTextEx(genshinFont, "A fragrant flower that will bloom",
                   (Vector2) {(float) panelX + 45, (float) cursorY + 20}, 24, 0, COL_TEXT_DARK);
        DrawTextEx(genshinFont, "for all eternity and never wither.",
                   (Vector2) {(float) panelX + 45, (float) cursorY + 50}, 24, 0, COL_TEXT_DARK);

        // Footer
        cursorY = panelY + panelH - 80;
        DrawRectangle(panelX, cursorY, panelW, 80, Fade(COL_PANEL_HEADER, 0.2f));
        DrawTextEx(genshinFont, "Equipped: Barbara", (Vector2) {(float) panelX + 100, (float) cursorY + 25}, 28, 0,
                   COL_TEXT_DARK);
        DrawCircle(panelX + 60, cursorY + 40, 25, WHITE);

        EndDrawing();
    }

    UnloadFont(genshinFont);
    CloseWindow();

    return 0;
}
