#pragma once

#include <cstdint>
#include <functional>
#include <string_view>

using TagSymbol = uint32_t;

inline TagSymbol toTagSymbol(std::string_view tag) {
  return static_cast<TagSymbol>(std::hash<std::string_view>{}(tag));
}