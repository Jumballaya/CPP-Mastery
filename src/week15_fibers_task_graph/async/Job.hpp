#pragma once

#include <cassert>
#include <cstddef>
#include <new>
#include <type_traits>
#include <utility>

template <size_t MaxSize = 64>
struct Job {
  using FuncPtr = void (*)(void*);
  using DestroyFuncPtr = void (*)(void*);

  alignas(std::max_align_t) char storage[MaxSize];
  FuncPtr func = nullptr;
  DestroyFuncPtr destroyFunc = nullptr;

  Job() = default;
  ~Job() {
    cleanup();
  }
  Job(const Job&) = delete;
  Job& operator=(const Job&) = delete;

  Job(Job&& other) noexcept {
    func = other.func;
    destroyFunc = other.destroyFunc;

    for (size_t i = 0; i < MaxSize; ++i) {
      storage[i] = other.storage[i];
    }

    other.func = nullptr;
    other.destroyFunc = nullptr;
  }

  Job& operator=(Job&& other) noexcept {
    if (this == &other) {
      return *this;
    }

    cleanup();
    func = other.func;
    destroyFunc = other.destroyFunc;
    for (size_t i = 0; i < MaxSize; ++i) {
      storage[i] = other.storage[i];
    }
    other.func = nullptr;
    other.destroyFunc = nullptr;

    return *this;
  }

  void operator()() const {
    assert(func && "Job function pointer is null");
    func((void*)storage);
  }

  template <typename Fn>
  void set(Fn&& fn) {
    static_assert(sizeof(Fn) <= MaxSize, "Lambda is too large for job storage");
    using LambdaType = std::decay_t<Fn>;

    func = [](void* ptr) { (*reinterpret_cast<LambdaType*>(ptr))(); };
    destroyFunc = [](void* ptr) { reinterpret_cast<LambdaType*>(ptr)->~LambdaType(); };

    new (storage) Fn(std::forward<Fn>(fn));
  }

 private:
  void cleanup() {
    if (destroyFunc) {
      destroyFunc((void*)storage);
      destroyFunc = nullptr;
    }
    func = nullptr;
  }
};