#pragma once

#include <thread>
#include <unordered_map>

#include "FrameArena.hpp"

class ThreadArenaRegistry {
 public:
  static void set(FrameArena* arena);
  static FrameArena* get();
  static void clear();

 private:
  static thread_local FrameArena* _tlsArena;
};