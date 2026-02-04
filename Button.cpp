
#include "raylib.h"

const int SCREEN_SIZE = 1600;
#include "raylib.h"

class Button {
  public:
    Rectangle   rect;
    const char* text;
    Color       baseColor;
    bool        isHovered;

    // Constructor to initialize the button
    Button(float x, float y, float w, float h, const char* label, Color color) {
        rect      = {x, y, w, h};
        text      = label;
        baseColor = color;
        isHovered = false;
    }

    // Logic: Returns true if the button was clicked
    bool Update(Vector2 mousePos) {
        isHovered = CheckCollisionPointRec(mousePos, rect);

        if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            return true;
        }
        return false;
    }

    // Visuals: Draws the button based on its state
    void Draw(Font font) {
        // Change color slightly if hovered
        Color drawColor = isHovered ? ColorBrightness(baseColor, 0.2f) : baseColor;

        DrawRectangleRec(rect, drawColor);
        DrawRectangleLinesEx(rect, 2, DARKGRAY); // Optional border

        // Center text logic (approximate)
        Vector2 textSize = MeasureTextEx(font, text, 40, 2);
        Vector2 textPos = {rect.x + (rect.width / 2) - (textSize.x / 2), rect.y + (rect.height / 2) - (textSize.y / 2)};

        DrawTextEx(font, text, textPos, 40, 2, WHITE);
    }
};

int main() {
    InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Raylib button");
    SetTargetFPS(60);

    Font genshinFont = LoadFontEx("resources/zh-cn.ttf", 96, 0, 0);
    SetTextureFilter(genshinFont.texture, TEXTURE_FILTER_BILINEAR);
    int mouse_left_clicked = 0, mouse_right_clicked = 0, button_clicked = 0;

    struct Button_prop {
        float posx, posy, width, height;
        Color bg_color;
    };

    Button_prop b1 = {600, 600, 300, 150, BLACK};
    Button      startButton(600, 800, 300, 100, "START", BLACK);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Vector2 mouse_position = GetMousePosition();
        DrawTextEx(genshinFont, TextFormat("Mouse position = { x: %.0f, y: %.0f }", mouse_position.x, mouse_position.y),
                   {100, 100}, 50, 5, BLACK);
        DrawTextEx(genshinFont, TextFormat("Mouse left clicked = %d", mouse_left_clicked), {100, 200}, 50, 5, BLACK);
        DrawTextEx(genshinFont, TextFormat("Mouse right clicked = %d", mouse_right_clicked), {100, 300}, 50, 5, BLACK);
        DrawTextEx(genshinFont, TextFormat("Button Clicked = %d", button_clicked), {100, 400}, 50, 5, BLACK);

        //
        Vector2 mousePos = GetMousePosition();

        if (startButton.Update(mousePos)) {
            // This code runs ONLY when the button is clicked
            // button_clicked++;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        DrawRectangle(b1.posx, b1.posy, b1.width, b1.height, b1.bg_color);
        DrawTextEx(genshinFont, "Click", {b1.posx + 80, b1.posy + 50}, 50, 5, WHITE);
        // Button click

        if (mouse_position.x >= b1.posx && mouse_position.x <= b1.posx + b1.width && mouse_position.y >= b1.posy
            && mouse_position.y <= b1.posy + b1.height) {

            // hover only
            b1.bg_color = GRAY;
            b1.width    = 350;
            b1.height   = 200;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                button_clicked++;
            }
        } else {
            b1.width    = 300;
            b1.height   = 150;
            b1.bg_color = BLACK;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            mouse_left_clicked++;
        }
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            mouse_right_clicked++;
        }

        startButton.Draw(genshinFont);
        EndDrawing();
    }

    UnloadFont(genshinFont);
    CloseWindow();
    return 0;
}
