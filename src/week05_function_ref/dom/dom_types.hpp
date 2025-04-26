#pragma once

#include <tuple>

enum class DOMEvents {
  Move,
  Change,
  Resize
};

using DOMFns = std::tuple<
    void(float, float),         // Move: (x, y)
    void(float, float, float),  // Change: (r, g, b)
    void(int, int)              // Resize: (w, h)
    >;
