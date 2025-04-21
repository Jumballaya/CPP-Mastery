#pragma once
#include <array>
#include <cstddef>

template <typename T, size_t N>
class ConstexprView {
 public:
  constexpr ConstexprView() = default;
  ~ConstexprView() = default;

  constexpr ConstexprView(const T* data) : _data(data) {};

  template <typename U, size_t M>
  constexpr ConstexprView(const std::array<U, M>& arr) : _data(arr.data()) {
    static_assert(M == N, "Array size must match view size");
  }

  const T& operator[](size_t index) const {
    return _data[index];
  }

  constexpr size_t size() const { return N; }
  constexpr const T* data() const { return _data; }

  constexpr const T* begin() const { return _data; }
  constexpr const T* end() const { return _data + N; }

  const T& front() const { return _data[0]; }
  const T& back() const { return _data[N - 1]; }

  constexpr static ConstexprView<T, N> from_array(const std::array<T, N>& arr) {
    return ConstexprView<T, N>(arr);
  }

 private:
  const T* _data = nullptr;
};