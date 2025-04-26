#pragma once

#include <memory>

#include "../EventDispatcher.hpp"
#include "../ListenerHandle.hpp"

template <typename Events, typename... Fns>
class EventEmitter {
 public:
  EventEmitter(std::shared_ptr<EventDispatcher<Events, Fns...>> dispatcher) : _dispatcher(dispatcher) {}

  template <typename... Args>
  void emit(Events tag, Args&&... args) {
    _dispatcher->emit(tag, std::forward<Args>(args)...);
  }

  template <typename... Args>
  void enqueue(Events tag, Args&&... args) {
    _dispatcher->enqueue(tag, std::forward<Args>(args)...);
  }

  template <typename Callable>
  ListenerHandle<Events, Fns...> addEventListener(Events tag, Callable&& cb) {
    return _dispatcher->addEventListener(tag, std::forward<Callable>(cb));
  }

  void removeEventListener(ListenerHandle<Events, Fns...>& handle) {
    _dispatcher->removeEventListener(handle);
  }

 private:
  std::shared_ptr<EventDispatcher<Events, Fns...>> _dispatcher;
};