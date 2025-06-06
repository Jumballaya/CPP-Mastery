#pragma once

#include <concepts>

#include "Component.hpp"

template <typename T>
concept ComponentType = std::derived_from<T, Component<T>>;