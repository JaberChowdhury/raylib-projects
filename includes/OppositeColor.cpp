#include "raylib.h"
#include <iostream>

Color OppositeColor(Color main_color) {
    int r = 256 - main_color.r;
    int g = 256 - main_color.g;
    int b = 256 - main_color.b;

    std::cout << "oposite color generated" << std::endl;
    return (Color) {(unsigned char) r, (unsigned char) g, (unsigned char) b};
}
