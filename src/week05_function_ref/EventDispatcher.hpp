#pragma once

#include <memory>

#include "EventBus.hpp"
#include "EventQueue.hpp"
#include "ListenerHandle.hpp"
#include "traits.hpp"

template <typename EventType, typename... Fns>
class EventDispatcher {
 public:
  EventDispatcher() : _bus(EventBus<EventType, Fns...>()), _queue({_bus}) {}
  ~EventDispatcher() = default;

  template <typename... Args>
  void emit(EventType tag, Args&&... args) {
    _bus.publish(tag, std::forward<Args>(args)...);
  }

  template <typename... Args>
  void enqueue(EventType tag, Args&&... args) {
    _queue.enqueue(tag, std::forward<Args>(args)...);
  }

  void dispatch_now() {
    _queue.dispatch_now();
  }

  //
  //  You HAVE to keep the reference to the handle alive for as long
  //  as you want the event listener to live, if you do not save the
  //  return value, your listener will silently dissapear
  //
  //    i'm still learning....
  //
  template <class Callable,
            class Fn = typename SelectMatchingFn<Callable, Fns...>::type,
            std::enable_if_t<!std::is_same_v<Fn, void>, int> = 0>
  ListenerHandle<EventType, Fns...>
  addEventListener(EventType tag, Callable&& cb) {
    uint32_t id = _bus.subscribe(tag, std::forward<Callable>(cb));
    return {&_bus, tag, id};
  }

  void removeEventListener(ListenerHandle<EventType, Fns...>& handle) {
    handle.unsubscribe();
  }

 private:
  EventBus<EventType, Fns...> _bus;
  EventQueue<EventType, Fns...> _queue;
};