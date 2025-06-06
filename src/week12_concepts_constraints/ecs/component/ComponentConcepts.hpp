#pragma once

#include <type_traits>

#include "Component.hpp"

template <typename T>
concept ComponentType = std::derived_from<T, Component<T>>;