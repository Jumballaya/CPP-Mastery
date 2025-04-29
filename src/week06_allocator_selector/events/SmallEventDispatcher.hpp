#pragma once

#include <cstddef>
#include <unordered_map>
#include <vector>

#include "AllocatedVariantCallback.hpp"
#include "Allocator.hpp"
#include "ListenerHandle.hpp"

template <typename EventType, size_t N, typename... Fns>
class SmallEventDispatcher {
 public:
  SmallEventDispatcher() : _bus(MessageBus<EventType, N, Fns...>()) {};
  ~SmallEventDispatcher() = default;

  template <class Callable,
            class Fn = typename SelectMatchingFn<Callable, Fns...>::type,
            std::enable_if_t<!std::is_same_v<Fn, void>, int> = 0>
  ListenerHandle<EventType, N, Fns...> addListener(EventType tag, Callable&& cb) {
    uint32_t id = _bus.subscribe(tag, std::forward<Callable>(cb));
    return {&_bus, tag, id};
  }

  bool removeListener(ListenerHandle<EventType, N, Fns...>& handle) {
    return handle.unsubscribe();
  }

  template <typename... Args>
  void emit(EventType tag, Args&&... args) {
    _bus.publish(tag, std::forward<Args>(args)...);
  }

  void clear() {
    _bus.clearAll();
  }

 private:
  MessageBus<EventType, N, Fns...> _bus;
};