#pragma once
#include <cstdint>
#include <raylib.h>
#include <vector>

// We ONLY use HCT, which we know you have.
#include "cam/hct.h"

using namespace material_color_utilities;

class MaterialUtils {
  private:
    double m_hue;
    double m_chroma;

  public:
    // Constructor
    MaterialUtils(Color c);
    MaterialUtils(uint32_t argb);

    // Core Logic
    void SetSeed(uint32_t argb);

    // Get specific tones (0-100)
    // We calculate these on-the-fly using HCT math
    Color GetPrimaryTone(double tone);
    Color GetSecondaryTone(double tone);
    Color GetTertiaryTone(double tone);
    Color GetNeutralTone(double tone);

    // Helpers
    static Color ArgbToRaylib(Argb argb);
    static Argb  RaylibToArgb(Color c);
};
