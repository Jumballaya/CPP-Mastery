#pragma once

#include <cassert>
#include <cstddef>
#include <cstring>
#include <functional>

#include "FunctionPolicy.hpp"

template <size_t N, typename Fn>
class SmallFunctionRef;

template <size_t N, typename R, typename... Args>
class SmallFunctionRef<N, R(Args...)> {
 public:
  template <typename Callable,
            typename = std::enable_if_t<SignatureTraits<R(Args...)>::template IsCompatible<Callable>::value>>
  SmallFunctionRef(Callable&& c) {
    assert(sizeof(c) <= N);
    new (&_storage) Callable(std::forward<Callable>(c));
    _callable = &_storage[0];
    _invoker = [](void* self, Args&&... args) -> R {
      return (*static_cast<Callable*>(self))(std::forward<Args>(args)...);
    };
    _destroyer = [](void* self) {
      static_cast<Callable*>(self)->~Callable();
    };
  };

  ~SmallFunctionRef() {
    if (_destroyer != nullptr) {
      _destroyer(_callable);
    }
    _destroyer = nullptr;
    _invoker = nullptr;
    _callable = nullptr;
  }

  SmallFunctionRef(const SmallFunctionRef&) = delete;
  SmallFunctionRef& operator=(const SmallFunctionRef&) = delete;

  SmallFunctionRef(SmallFunctionRef&& other) {
    std::memcpy(&_storage, &other._storage, N);
    _callable = &_storage[0];
    _invoker = other._invoker;
    _destroyer = other._destroyer;

    other._callable = nullptr;
    other._invoker = nullptr;
    other._destroyer = nullptr;
  }

  SmallFunctionRef& operator=(SmallFunctionRef&& other) {
    if (this == &other) return *this;
    if (_destroyer != nullptr) {
      _destroyer(_callable);
    }

    std::memcpy(&_storage, &other._storage, N);
    _callable = &_storage[0];
    _invoker = other._invoker;
    _destroyer = other._destroyer;

    other._callable = nullptr;
    other._invoker = nullptr;
    other._destroyer = nullptr;

    return *this;
  }

  R operator()(Args&&... args) {
    if (!_invoker) {
      throw std::bad_function_call();
    }
    return _invoker(_callable, std::forward<Args>(args)...);
  }

 private:
  alignas(std::max_align_t) unsigned char _storage[N];
  void* _callable = nullptr;
  R (*_invoker)(void*, Args&&...) = nullptr;
  void (*_destroyer)(void*) = nullptr;
};