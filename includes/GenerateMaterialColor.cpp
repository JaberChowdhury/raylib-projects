#include "raylib.h"
#include <cmath>

Color GenerateMaterialColor(int hue, int shade) {

    // Validate hue and shade
    if (shade <= 0) {
        shade = 50;
    }
    if (shade > 900) {
        shade = 900;
    }
    if (hue < 0 || hue > 360) {
        // Return black or a default error color
        return (Color) {0, 0, 0, 255};
    }

    // Calculate "Lightness" (Acts as Value in HSV model based on your logic)
    double value = 1.0 - (shade / 1000.0);

    // Material Design often keeps the 500 shade as the base mid-tone
    if (shade == 500)
        value = 0.5;

    // Saturation is fixed at 0.5 in your logic
    double saturation = 0.5;

    // HSV to RGB Conversion
    int h_sector = hue / 60;
    // Handle hue 360 as 0
    if (h_sector >= 6)
        h_sector = 0;

    // Calculate fractional part of hue sector
    double f = (hue / 60.0) - (int) (hue / 60.0);

    double p = value * (1 - saturation);
    double q = value * (1 - f * saturation);
    double t = value * (1 - (1 - f) * saturation);

    int r = 0, g = 0, b = 0;

    switch (h_sector) {
    case 0:
        r = value * 255;
        g = t * 255;
        b = p * 255;
        break;
    case 1:
        r = q * 255;
        g = value * 255;
        b = p * 255;
        break;
    case 2:
        r = p * 255;
        g = value * 255;
        b = t * 255;
        break;
    case 3:
        r = p * 255;
        g = q * 255;
        b = value * 255;
        break;
    case 4:
        r = t * 255;
        g = p * 255;
        b = value * 255;
        break;
    case 5:
        r = value * 255;
        g = p * 255;
        b = q * 255;
        break;
    default:
        r = value * 255;
        g = p * 255;
        b = q * 255;
        break;
    }

    return (Color) {(unsigned char) r, (unsigned char) g, (unsigned char) b, 255};
}
