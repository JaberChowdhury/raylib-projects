#include "raylib.h"
class Card {

  public:
    int x, y;
    Card(int posx, int posy) {
        x = posx;
        y = posy;
    }

    void draw() { DrawRectangle(this->x, this->y, this->w, int height, Color color) }

    ~Card() { delete this; }
};
