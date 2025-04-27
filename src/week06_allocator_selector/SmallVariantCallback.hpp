#pragma once

#include <cstddef>
#include <variant>

#include "FunctionPolicy.hpp"
#include "SmallFunctionRef.hpp"

template <size_t N, typename... Fns>
class SmallVariantCallback {
 public:
  SmallVariantCallback() = default;
  ~SmallVariantCallback() = default;

  template <typename Callable>
  SmallVariantCallback(Callable&& c) {
    using Match = typename SelectMatchingFn<Callable, Fns...>::type;
    static_assert(!std::is_same_v<Match, void>, "Callable type incompatible!");
    _func.template emplace<SmallFunctionRef<N, Match>>(std::forward<Callable>(c));
  }

  SmallVariantCallback(const SmallVariantCallback&) = delete;
  SmallVariantCallback& operator=(const SmallVariantCallback&) = delete;

  SmallVariantCallback(SmallVariantCallback&& other) noexcept
      : _func(std::move(other._func)) {}

  SmallVariantCallback& operator=(SmallVariantCallback&& other) noexcept {
    if (this == &other) return *this;
    _func = std::move(other._func);
    return *this;
  }

  template <typename... Args>
  auto operator()(Args&&... args) const {
    return std::visit(
        [&](auto&& fn) -> decltype(auto) {
          using FnType = std::decay_t<decltype(fn)>;
          if constexpr (std::is_same_v<FnType, std::monostate>) {
            throw std::bad_function_call();
          } else {
            return fn(std::forward<Args>(args)...);
          }
        },
        _func);
  }

 private:
  std::variant<std::monostate, SmallFunctionRef<N, Fns>...> _func;
};