#pragma once

#include <cstddef>
#include <memory>
#include <variant>

#include "../FunctionPolicy.hpp"
#include "AllocatedFunctionRef.hpp"
#include "Allocator.hpp"

template <size_t N, typename... Fns>
class AllocatedVariantCallback {
 public:
  AllocatedVariantCallback() = default;
  ~AllocatedVariantCallback() = default;

  AllocatedVariantCallback(const AllocatedVariantCallback&) = delete;
  AllocatedVariantCallback& operator=(const AllocatedVariantCallback&) = delete;

  AllocatedVariantCallback(AllocatedVariantCallback&& other) noexcept
      : _func(std::move(other._func)), _allocator(std::move(other._allocator)) {}

  AllocatedVariantCallback& operator=(AllocatedVariantCallback&& other) noexcept {
    if (this == &other) return *this;
    _func = std::move(other._func);
    _allocator = std::move(other._allocator);
    return *this;
  }

  template <typename Callable>
  AllocatedVariantCallback(Callable&& c, std::shared_ptr<Allocator<N>> allocator) : _allocator(std::move(allocator)) {
    using Match = typename SelectMatchingFn<Callable, Fns...>::type;
    static_assert(!std::is_same_v<Match, void>, "Callable type incompatible!");
    _func.template emplace<AllocatedFunctionRef<Allocator<N>, Match>>(_allocator, std::forward<Callable>(c));
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
  std::variant<std::monostate, AllocatedFunctionRef<Allocator<N>, Fns>...> _func;
  std::shared_ptr<Allocator<N>> _allocator;
};