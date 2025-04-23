#pragma once

#include <tuple>
#include <variant>

template <typename Fn>
struct SignatureTraits;

template <typename R, typename... Args>
struct SignatureTraits<R(Args...)> {
  using Return = R;
  using ArgsTuple = std::tuple<Args...>;

  template <typename Callable>
  using IsCompatible = std::is_invocable_r<R, Callable, Args...>;
};

template <typename Callable, typename... Fns>
struct SelectMatchingFn {
  using type = void;
};

template <typename Callable, typename Fn, typename... Rest>
struct SelectMatchingFn<Callable, Fn, Rest...> {
  using type = std::conditional_t<
      SignatureTraits<Fn>::template IsCompatible<Callable>::value,
      Fn,
      typename SelectMatchingFn<Callable, Rest...>::type>;
};