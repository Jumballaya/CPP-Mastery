#pragma once

#include <variant>

#include "FunctionRef.hpp"

template <typename... Fns> class VariantCallback {
public:
  VariantCallback() = default;

  template <typename F> VariantCallback(FunctionRef<F> fn) : _func(fn) {}

  template <typename Callable>
  VariantCallback(Callable &&c) : _func(FunctionRef(c)) {}

  auto operator()(Args &&...args);

  int index() const;

  template <typename T> bool holds() const;

  bool empty() const;

  void reset();

private:
  // std::monostate is being used to signal an empty state, so index() == 0
  // means emptys
  std::variant<std::monostate, <Fns>...> _func;
};