#pragma once

#include <cassert>
#include <type_traits>
#include <utility>

template <size_t MaxSize = 128>
struct Job {
 private:
  struct Base {
    virtual ~Base() = default;
    virtual void execute() = 0;
    virtual void moveTo(void* dest) = 0;
  };

  template <typename Fn>
  struct Model : Base {
    Fn fn;

    explicit Model(Fn&& f) : fn(std::move(f)) {}
    void execute() override { fn(); }
    void moveTo(void* dest) override {
      new (dest) Model<Fn>(std::move(fn));
    }
  };

  alignas(std::max_align_t) char storage[MaxSize];
  Base* base = nullptr;

 public:
  Job() = default;
  ~Job() { reset(); }

  Job(const Job&) = delete;
  Job& operator=(const Job&) = delete;

  Job(Job&& other) noexcept {
    if (other.base) {
      other.base->moveTo(storage);
      base = reinterpret_cast<Base*>(storage);
      other.base = nullptr;
    }
  }

  Job& operator=(Job&& other) noexcept {
    if (this != &other) {
      reset();
      if (other.base) {
        other.base->moveTo(storage);
        base = reinterpret_cast<Base*>(storage);
        other.base = nullptr;
      }
    }
    return *this;
  }

  template <typename Fn>
  void set(Fn&& fn) {
    static_assert(sizeof(Model<Fn>) <= MaxSize, "Job is too large for storage");
    new (storage) Model<Fn>(std::forward<Fn>(fn));
    base = reinterpret_cast<Base*>(storage);
  }

  void operator()() {
    assert(base && "Job not set");
    base->execute();
  }

  void reset() {
    if (base) {
      base->~Base();
      base = nullptr;
    }
  }

  bool valid() const { return base != nullptr; }
};