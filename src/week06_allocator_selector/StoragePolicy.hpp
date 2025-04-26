#pragma once

#include <cassert>
#include <cstddef>

struct HeapStorage {};

template <size_t N>
struct InlineStorage {};

template <typename StoragePolicy>
struct AllocatorSelector;

template <>
struct AllocatorSelector<HeapStorage> {
  struct type {
    template <typename T, typename... Args>
    static T* allocate(Args&&... args) {
      return new T(std::forward<Args>(args)...);
    }

    template <typename T>
    static void deallocate(T* ptr) {
      delete ptr;
    }
  };
};

template <size_t N>
struct AllocatorSelector<InlineStorage<N>> {
  struct type {
    template <typename T, typename... Args>
    static T* allocate(Args&&... args) {
      auto& self = instance();
      assert((sizeof(T) <= N) && "Not enough space allocated");
      assert(!self.used && "InlineStorage already used!");
      self.used = true;
      return new (&self.buffer) T(std::forward<Args>(args)...);
    }

    template <typename T>
    static void deallocate(T* ptr) {
      auto& self = instance();
      if (!self.used) return;
      ptr->~T();
      self.used = false;
    }

   private:
    bool used;
    alignas(std::max_align_t) unsigned char buffer[N];
  };

 private:
  static type& instance() {
    static type inst;
    return inst;
  }
};