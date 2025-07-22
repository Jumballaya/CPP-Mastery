#include "ThreadArenaRegistry.hpp"

thread_local FrameArena* ThreadArenaRegistry::_tlsArena = nullptr;

void ThreadArenaRegistry::set(FrameArena* arena) {
  _tlsArena = arena;
}

FrameArena* ThreadArenaRegistry::get() {
  return _tlsArena;
}

void ThreadArenaRegistry::clear() {
  _tlsArena = nullptr;
}