#pragma once

#include <type_traits>

template <typename... Types>
struct TypeList {};

using EmptyList = TypeList<>;

//
//  TypeList contains helper
//
//  Example Usage:
//    static_assert(contains<int, TypeList<float, int, double>>::value, "int should be found");
//    static_assert(!contains<char, TypeList<float, int, double>>::value, "char should not be found");
//

// Match not found
template <typename T, typename List>
struct contains : std::false_type {};

// Match found
template <typename T, typename... Tail>
struct contains<T, TypeList<T, Tail...>> : std::true_type {};

// Recursive call
template <typename T, typename Head, typename... Tail>
struct contains<T, TypeList<Head, Tail...>> : contains<T, TypeList<Tail...>> {};

//
//  Iterating through TypeList
//
//  Example Usage:
//      struct PrintType {
//          template <typename T>
//          void operator()() {
//              std::cout << typeid(T).name() << std::endl;
//          }
//      };
//
//      TypeListForEach<TypeList<int, float, double>>::apply(PrintType{});
//

template <typename List>
struct TypeListForEach;

template <typename Head, typename... Tail>
struct TypeListForEach<TypeList<Head, Tail...>> {
  template <typename Func>
  static void apply(Func&& func) {
    func.template operator()<Head>();
    TypeListForEach<TypeList<Tail...>>::apply(std::forward<Func>(func));
  }
};

// Empty list (empty or at the end of the iteration)
template <>
struct TypeListForEach<TypeList<>> {
  template <typename Func>
  static void apply(Func&&) {}
};
