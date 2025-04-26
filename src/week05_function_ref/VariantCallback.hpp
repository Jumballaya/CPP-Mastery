#pragma once

#include <functional>

#include "FunctionRef.hpp"
#include "traits.hpp"

template <typename... Fns>
class VariantCallback {
 public:
  VariantCallback() = default;

  template <class Callable,
            class Fn = typename SelectMatchingFn<Callable, Fns...>::type>
  VariantCallback(Callable&& c) {
    static_assert(!std::is_same_v<Fn, void>, "Callable incompatible");

    if constexpr (std::is_lvalue_reference_v<Callable&&>) {
      // l-value: fast, non-owning view
      _func.template emplace<FunctionRef<Fn>>(c);
    } else {
      // r-value: own a copy
      _func.template emplace<std::function<Fn>>(std::move(c));
    }
  }

  VariantCallback(const VariantCallback&) = default;
  VariantCallback(VariantCallback&&) noexcept = default;
  VariantCallback& operator=(const VariantCallback&) = default;
  VariantCallback& operator=(VariantCallback&&) noexcept = default;

  template <typename... Args>
  void operator()(Args&&... args) const {
    std::visit(
        [&](auto&& fn) {
          using FnType = std::decay_t<decltype(fn)>;
          if constexpr (std::is_same_v<FnType, std::monostate>) {
            throw std::bad_function_call();
          } else if constexpr (std::is_invocable_r_v<void, FnType, Args&&...>) {
            fn(std::forward<Args>(args)...);
          }
        },
        _func);
  }

  explicit operator bool() const { return has_value(); }

  int index() const {
    int raw = _func.index();
    return raw == 0 ? -1 : raw - 1;
  }

  bool has_value() const { return index() != 0; }
  void reset() { _func.template emplace<std::monostate>(); }

 private:
  // std::monostate is being used to signal an empty state, so index() == 0 means empty
  std::variant<std::monostate, FunctionRef<Fns>..., std::function<Fns>...> _func;
};