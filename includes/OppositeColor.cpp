#include "raylib.h"

Color OppositeColor(Color main_color) {
    int r = r > main_color.r ? (r - main_color.r) : (main_color.r - r),
        g = g > main_color.g ? (g - main_color.g) : (main_color.g - g),
        b = b > main_color.b ? (b - main_color.b) : (main_color.b - b);
    return (Color) {(unsigned char) r, (unsigned char) g, (unsigned char) b};
}
