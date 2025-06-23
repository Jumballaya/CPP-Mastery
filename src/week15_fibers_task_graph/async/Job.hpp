#pragma once

#include <cassert>
#include <new>
#include <type_traits>
#include <utility>

template <size_t MaxSize = 64>
struct Job {
  using FuncPtr = void (*)(void*);

  alignas(std::max_align_t) char storage[MaxSize];
  FuncPtr func = nullptr;

  void operator()() const {
    assert(func && "Job function pointer is null");
    func((void*)storage);
  }

  template <typename Fn>
  void set(Fn&& fn) {
    static_assert(sizeof(Fn) <= MaxSize, "Lambda is too large for job storage");
    func = [](void* ptr) { (*reinterpret_cast<Fn*>(ptr))(); };
    new (storage) Fn(std::forward<Fn>(fn));
  }
};