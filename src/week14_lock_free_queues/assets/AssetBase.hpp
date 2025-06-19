#pragma once

#include <typeindex>

class AssetBase {
 public:
  virtual ~AssetBase() = default;
  virtual std::type_index type() const = 0;
};