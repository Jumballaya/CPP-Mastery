#pragma once

#include <functional>

#include "FunctionRef.hpp"
#include "traits.hpp"

template <typename... Fns>
class VariantCallback {
 public:
  VariantCallback() = default;

  template <typename Callable,
            typename Fn = typename SelectMatchingFn<Callable, Fns...>::type,
            typename = std::enable_if_t<
                !std::is_same_v<Fn, void> &&
                std::is_lvalue_reference_v<Callable>>>
  VariantCallback(Callable&& c) {
    _func.template emplace<FunctionRef<Fn>>(std::forward<Callable>(c));
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

  explicit operator bool() const { return has_value(); }
  int index() const { return _func.index(); }
  bool has_value() const { return index() != 0; }
  void reset() { _func.template emplace<std::monostate>(); }

 private:
  // std::monostate is being used to signal an empty state, so index() == 0 means empty
  std::variant<std::monostate, FunctionRef<Fns>...> _func;
};