#pragma once

#include <bitset>
#include <cstddef>
#include <functional>
#include <memory>
#include <new>
#include <utility>

#include "PoolAllocator.hpp"
#include "PoolResource.hpp"

// handles/ids	Indirect access with safety/stale checking
// reset()	Bulk wipe with optional auto-destruction

template <typename T, size_t N>
class Pool {
  template <bool IsConst>
  class Iterator {
   public:
    using PoolType = std::conditional_t<IsConst, const Pool<T, N>, Pool<T, N>>;

    using ValueType = std::conditional_t<IsConst, const T, T>;
    using Reference = ValueType&;
    using Pointer = ValueType*;

    Iterator(PoolType* pool, size_t index) : _pool(pool), _index(index) {
      skipToNextValid();
    }

    Reference operator*() const {
      return *(_pool->resolve(_index));
    }

    Pointer operator->() const {
      return _pool->resolve(_index);
    }

    Iterator& operator++() {
      ++_index;
      skipToNextValid();
      return *this;
    }

    bool operator!=(const Iterator& other) const {
      return _index != other._index;
    }

   private:
    PoolType* _pool;
    size_t _index;

    void skipToNextValid() {
      while (_index < N && !_pool->_used.test(_index)) {
        ++_index;
      }
    }
  };

 public:
  using iterator = Iterator<false>;
  using const_iterator = Iterator<true>;

  Pool() : _allocator(_buffer, PoolAllocator::slotSizeFor<T>(), N) {}

  ~Pool() {
    clear();
  };

  Pool(const Pool&) = delete;
  Pool& operator=(const Pool&) = delete;

  template <typename... Args>
  T* create(Args&&... args) {
    void* mem = _allocator.allocate();
    if (!mem) throw std::bad_alloc();
    T* obj = new (mem) T(std::forward<Args>(args)...);

    size_t idx = _allocator.indexOf(obj);
    _used.set(idx);

    return obj;
  }

  void destroy(T* obj) {
    if (obj) {
      size_t idx = _allocator.indexOf(obj);
      obj->~T();
      _allocator.deallocate(obj);
      _used.reset(idx);
    }
  }

  void clear() {
    for (size_t i = 0; i < N; ++i) {
      if (_used.test(i)) {
        std::byte* slotBase = _buffer + i * PoolAllocator::slotSizeFor<T>();
        T* obj = reinterpret_cast<T*>(slotBase + offsetof(PoolAllocator::Slot, userData));
        obj->~T();
        _allocator.deallocate(obj);
        _used.reset(i);
      }
    }
  }

  T* resolve(size_t i) {
    std::byte* base = _buffer + i * PoolAllocator::slotSizeFor<T>();
    return reinterpret_cast<T*>(base + offsetof(PoolAllocator::Slot, userData));
  }

  const T* resolve(size_t i) const {
    const std::byte* base = _buffer + i * PoolAllocator::slotSizeFor<T>();
    return reinterpret_cast<const T*>(base + offsetof(PoolAllocator::Slot, userData));
  }

  size_t used() const { return _allocator.used(); }
  size_t capacity() const { return _allocator.capacity(); }
  bool empty() const { return used() == 0; }

  iterator begin() { return iterator(this, 0); }
  iterator end() { return iterator(this, N); }

  const_iterator begin() const { return const_iterator(this, 0); }
  const_iterator end() const { return const_iterator(this, N); }

 private:
  alignas(std::max_align_t) std::byte _buffer[PoolAllocator::slotSizeFor<T>() * N];
  PoolAllocator _allocator;

  std::bitset<N> _used;
};