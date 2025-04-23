#pragma once

#include <type_traits>
#include <utility>

template <typename>
class FunctionRef;

template <typename R, typename... Args>
class FunctionRef<R(Args...)> {
 public:
  template <typename Callable>
  FunctionRef(Callable&&) = delete;

  template <typename Callable>
  FunctionRef(Callable& callable) noexcept {
    static_assert(std::is_invocable_r_v<R, Callable&, Args...>,
                  "Callable must be invocable with Args... and return R");

    _callable = static_cast<void*>(&callable);

    _erased = [](void* obj, Args... args) -> R {
      return (*static_cast<Callable*>(obj))(std::forward<Args>(args)...);
    };
  }

  FunctionRef(const FunctionRef&) noexcept = default;
  FunctionRef& operator=(const FunctionRef&) noexcept = default;
  FunctionRef(FunctionRef&&) noexcept = default;
  FunctionRef& operator=(FunctionRef&&) noexcept = default;

  R operator()(Args... args) const {
    return _erased(_callable, std::forward<Args>(args)...);
  }

  explicit operator bool() const noexcept { return _erased != nullptr; }

 private:
  void* _callable = nullptr;
  R (*_erased)(void*, Args...) = nullptr;
};