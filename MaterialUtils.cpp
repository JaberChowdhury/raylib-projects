#include "MaterialUtils.hpp"

MaterialUtils::MaterialUtils(Color c)
    : MaterialUtils(RaylibToArgb(c)) {}

MaterialUtils::MaterialUtils(uint32_t argb) {
    SetSeed(argb);
}

void MaterialUtils::SetSeed(uint32_t argb) {
    // 1. Convert input to HCT to sanitize it
    Hct seed(argb);

    // 2. Store the perceptually accurate Hue and Chroma
    m_hue    = seed.get_hue();
    m_chroma = seed.get_chroma();
}

Color MaterialUtils::GetPrimaryTone(double tone) {
    // Primary = Seed Hue + Seed Chroma
    Hct color(m_hue, m_chroma, tone);
    return ArgbToRaylib(color.ToInt());
}

Color MaterialUtils::GetSecondaryTone(double tone) {
    // Secondary = Seed Hue + (Chroma / 3)
    // This matches standard Material Design logic
    Hct color(m_hue, m_chroma / 3.0, tone);
    return ArgbToRaylib(color.ToInt());
}

Color MaterialUtils::GetTertiaryTone(double tone) {
    // Tertiary = (Seed Hue + 60) + (Chroma / 2)
    Hct color(m_hue + 60.0, m_chroma / 2.0, tone);
    return ArgbToRaylib(color.ToInt());
}

Color MaterialUtils::GetNeutralTone(double tone) {
    // Neutral = Seed Hue + Low Chroma (usually 4 to 6)
    Hct color(m_hue, 6.0, tone);
    return ArgbToRaylib(color.ToInt());
}

Color MaterialUtils::ArgbToRaylib(Argb argb) {
    unsigned char a = (argb >> 24) & 0xFF;
    unsigned char r = (argb >> 16) & 0xFF;
    unsigned char g = (argb >> 8) & 0xFF;
    unsigned char b = (argb) & 0xFF;
    return {r, g, b, a};
}

Argb MaterialUtils::RaylibToArgb(Color c) {
    return (c.a << 24) | (c.r << 16) | (c.g << 8) | c.b;
}
