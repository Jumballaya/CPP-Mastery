#pragma once

#include <cstdint>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "EventBus.hpp"

template <typename... Fns>
class EventQueue {
 public:
  EventQueue(EventBus<Fns...>& bus) : _bus(bus) {};
  ~EventQueue() = default;

  template <typename... Args>
  void enqueue(EventTag tag, Args&&... args) {
    auto tuple = std::make_tuple(std::forward<Args>(args)...);
    _events.emplace_back([this, tag, tuple = std::move(tuple)]() mutable {
      std::apply(
          [this, tag](auto&&... unpackedArgs) {
            _bus.template publish(tag, std::forward<decltype(unpackedArgs)>(unpackedArgs)...);
          },
          tuple);
    });
  }

  void dispatch_now() {
    for (auto& evt : _events) {
      evt();
    }
    clear();
  }

  void clear() noexcept { _events.clear(); }
  size_t size() const { return _events.size(); }
  bool empty() const { return _events.empty(); }

 private:
  EventBus<Fns...>& _bus;
  std::vector<std::function<void()>> _events;
};