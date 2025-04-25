#pragma once

#include <cstdint>
#include <vector>

#include "FunctionRef.hpp"
#include "traits.hpp"

template <typename Fn>
class MultiCallback {
 public:
  using ListenerID = uint32_t;

  MultiCallback() = default;
  ~MultiCallback() = default;

  template <typename Callable>
  ListenerID add(Callable& cb) {
    static_assert(SignatureTraits<Fn>::template IsCompatible<Callable>::value, "Listener must match function signature");
    _listeners.push_back({_nextID, FunctionRef<Fn>(cb)});
    return _nextID++;
  }

  bool remove(ListenerID id) {
    auto newEnd = std::remove_if(_listeners.begin(), _listeners.end(), [&](const Entry& e) { return e.id == id; });
    bool removed = newEnd != _listeners.end();
    _listeners.erase(newEnd, _listeners.end());
    return removed;
  }

  void clear() {
    _listeners.clear();
  }

  template <typename... Args>
  void operator()(Args&&... args) {
    for (auto& listener : _listeners) {
      listener.fn(std::forward<Args>(args)...);
    }
  }

  size_t size() const { return _listeners.size(); }
  bool empty() const { return _listeners.empty(); }

 private:
  struct Entry {
    ListenerID id;
    FunctionRef<Fn> fn;
  };

  std::vector<Entry> _listeners;
  ListenerID _nextID = 1;
};