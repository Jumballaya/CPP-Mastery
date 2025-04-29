#pragma once

#include <cassert>
#include <cstddef>
#include <cstring>
#include <functional>
#include <memory>

#include "../FunctionPolicy.hpp"

template <typename Alloc, typename Fn>
class AllocatedFunctionRef;

template <typename Alloc, typename R, typename... Args>
class AllocatedFunctionRef<Alloc, R(Args...)> {
 public:
  template <typename Callable>
  AllocatedFunctionRef(std::shared_ptr<Alloc> alloc, Callable&& c) {
    static_assert(sizeof(Callable) <= Alloc::kMaxSize, "Callable too large");
    _allocator = std::move(alloc);
    _callable = _allocator->allocate(sizeof(Callable), alignof(Callable));
    new (_callable) Callable(std::forward<Callable>(c));
    _invoker = [](void* self, Args&&... args) -> R {
      return (*static_cast<Callable*>(self))(std::forward<Args>(args)...);
    };
    _destroyer = [](void* self) {
      static_cast<Callable*>(self)->~Callable();
    };
  }

  ~AllocatedFunctionRef() {
    if (_destroyer != nullptr) {
      _destroyer(_callable);
      _allocator->deallocate(_callable);
    }
    _callable = nullptr;
    _invoker = nullptr;
    _destroyer = nullptr;
    _allocator = nullptr;
  }

  AllocatedFunctionRef(const AllocatedFunctionRef&) = delete;
  AllocatedFunctionRef& operator=(const AllocatedFunctionRef&) = delete;

  AllocatedFunctionRef(AllocatedFunctionRef&& other) noexcept {
    _callable = other._callable;
    _invoker = other._invoker;
    _destroyer = other._destroyer;
    _allocator = other._allocator;

    other._callable = nullptr;
    other._invoker = nullptr;
    other._destroyer = nullptr;
    other._allocator = nullptr;
  }

  AllocatedFunctionRef& operator=(AllocatedFunctionRef&& other) noexcept {
    if (this == &other) return *this;
    if (_destroyer != nullptr) {
      _destroyer(_callable);
      _allocator->deallocate(_callable);
    }

    _callable = other._callable;
    _invoker = other._invoker;
    _destroyer = other._destroyer;
    _allocator = other._allocator;

    other._callable = nullptr;
    other._invoker = nullptr;
    other._destroyer = nullptr;
    other._allocator = nullptr;

    return *this;
  }

  R operator()(Args&&... args) const {
    if (!_invoker || !_callable) {
      throw std::bad_function_call();
    }
    return _invoker(_callable, std::forward<Args>(args)...);
  }

 private:
  void* _callable = nullptr;
  R (*_invoker)(void*, Args&&...) = nullptr;
  void (*_destroyer)(void*) = nullptr;
  std::shared_ptr<Alloc> _allocator = nullptr;
};