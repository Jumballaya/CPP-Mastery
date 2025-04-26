#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "VariantCallback.hpp"
#include "traits.hpp"

template <typename EventType, typename... Fns>
class EventBus {
 public:
  EventBus() = default;
  ~EventBus() = default;

  using Listener = VariantCallback<Fns...>;
  using ListenerID = uint32_t;

  template <class Callable,
            class Fn = typename SelectMatchingFn<Callable, Fns...>::type,
            std::enable_if_t<!std::is_same_v<Fn, void>, int> = 0>
  ListenerID subscribe(EventType tag, Callable&& cb) {
    Entry e;
    e.id = _nextID++;
    e.fn = VariantCallback<Fns...>(std::forward<Callable>(cb));  // ← forward!
    _table[tag].push_back(std::move(e));
    return e.id;
  }

  template <typename... Args>
  void publish(EventType tag, Args&&... args) {
    auto it = _table.find(tag);
    if (it == _table.end()) return;

    for (auto& entry : it->second) {
      entry.fn(std::forward<Args>(args)...);
    }
  }

  bool unsubscribe(EventType tag, ListenerID id) {
    auto it = _table.find(tag);
    if (it == _table.end()) return false;

    auto& listeners = it->second;

    auto newEnd = std::remove_if(listeners.begin(), listeners.end(), [&](const Entry& e) { return e.id == id; });
    bool removed = newEnd != listeners.end();
    listeners.erase(newEnd, listeners.end());

    if (listeners.empty()) _table.erase(it);
    return removed;
  }

  void clear(EventType tag) {
    if (_table.find(tag) == _table.end()) return;
    _table[tag].clear();
  }

  void clearAll() {
    _table.clear();
  }

  size_t listenerCount(EventType tag) const {
    if (_table.find(tag) == _table.end()) return 0;
    return _table[tag].size();
  }

 private:
  struct Entry {
    ListenerID id;
    Listener fn;
  };

  std::unordered_map<EventType, std::vector<Entry>> _table;
  ListenerID _nextID = 1;
};