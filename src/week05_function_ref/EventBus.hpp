#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "VariantCallback.hpp"
#include "traits.hpp"

enum class EventTag : uint32_t {
  Log,
  Damage,
  Click
};

template <typename... Fns>
class EventBus {
 public:
  EventBus() = default;
  ~EventBus() = default;

  using Listener = VariantCallback<Fns...>;
  using ListenerID = uint32_t;

  template <typename Callable,
            typename Fn = typename SelectMatchingFn<Callable, Fns...>::type,
            typename = std::enable_if_t<
                !std::is_same_v<Fn, void> &&
                std::is_lvalue_reference_v<Callable>>>
  ListenerID subscribe(EventTag tag, Callable&& cb) {
    _table[tag].push_back(Entry{_nextID, cb});
    return _nextID++;
  }

  template <typename... Args>
  void publish(EventTag tag, Args&&... args) {
    auto it = _table.find(tag);
    if (it == _table.end()) return;

    for (auto& entry : it->second) {
      entry.fn(std::forward<Args>(args)...);
    }
  }

  bool unsubscribe(EventTag tag, ListenerID id) {
    auto it = _table.find(tag);
    if (it == _table.end()) return false;

    auto& listeners = it->second;

    auto newEnd = std::remove_if(listeners.begin(), listeners.end(), [&](const Entry& e) { return e.id == id; });
    bool removed = newEnd != listeners.end();
    listeners.erase(newEnd, listeners.end());

    if (listeners.empty()) _table.erase(it);
    return removed;
  }

  void clear(EventTag tag) {
    if (_table.find(tag) == _table.end()) return;
    _table[tag].clear();
  }

  void clearAll() {
    _table.clear();
  }

  size_t listenerCount(EventTag tag) const {
    if (_table.find(tag) == _table.end()) return 0;
    return _table[tag].size();
  }

 private:
  struct Entry {
    ListenerID id;
    Listener fn;
  };

  std::unordered_map<EventTag, std::vector<Entry>> _table;
  ListenerID _nextID = 1;
};