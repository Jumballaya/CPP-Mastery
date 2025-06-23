#pragma once

struct Job {
  void (*func)(void*);
  void* data;

  void operator()() const {
    func(data);
  }

  template <typename Fn>
  void set(Fn&& fn) {
    static_assert(sizeof(Fn) <= sizeof(data), "Job captured is too large");
    func = [](void* ptr) { (*reinterpret_cast<Fn*>(ptr))(); };
    new (data) Fn(std::forward<Fn>(fn));
  }
};