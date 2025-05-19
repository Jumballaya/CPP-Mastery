#pragma once

#include <memory_resource>

#include "./FrameAllocator.hpp"

class FrameResource : std::pmr::memory_resource {
};