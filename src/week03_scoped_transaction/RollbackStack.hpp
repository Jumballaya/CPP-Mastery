#pragma once

#include <cassert>
#include <vector>

template <typename T>
class RollbackStack {
 public:
  RollbackStack() = default;
  ~RollbackStack() = default;

  RollbackStack(const RollbackStack& other) = delete;
  RollbackStack& operator=(const RollbackStack& other) = delete;

  RollbackStack(RollbackStack&& other) : _stack(std::move(other._stack)) {}
  RollbackStack& operator=(RollbackStack&& other) {
    if (this == &other) return *this;
    _stack = std::move(other._stack);
    return *this;
  }

  void push(T&& snapshot) { _stack.push_back(std::move(snapshot)); }
  void push(const T& snapshot) { _stack.push_back(snapshot); }

  bool pop() {
    if (size() <= 0) return false;
    _stack.pop_back();
    return size() > 0;
  }

  bool pop_to(size_t index) {
    if (index >= _stack.size()) return false;
    _stack.resize(index + 1);
    return true;
  }

  const T& current() const {
    assert(!_stack.empty() && "RollbackStack: calling current() on empty stack.");
    return _stack.back();
  }

  const T& peek(size_t offset = 0) const {
    assert((offset < _stack.size()) && "RollbackStack: calling peek with offset out of bounds");
    return _stack[_stack.size() - 1 - offset];
  }

  size_t size() const { return _stack.size(); }
  bool empty() const { return _stack.empty(); }
  void clear() { _stack.clear(); };

 private:
  std::vector<T> _stack;
};