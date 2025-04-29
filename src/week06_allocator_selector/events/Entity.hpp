#pragma once

#include <vector>

#include "ListenerHandle.hpp"
#include "SmallEventDispatcher.hpp"

template <typename EventType, size_t N, typename... Fns>
class Entity {
 public:
  using Dispatcher = SmallEventDispatcher<EventType, N, Fns...>;
  using Handle = ListenerHandle<EventType, N, Fns...>;

  explicit Entity(Dispatcher& dispatcher) : _dispatcher(dispatcher) {}

  template <typename Callable>
  void on(EventType tag, Callable&& cb) {
    auto handle = _dispatcher.addListener(tag, std::forward<Callable>(cb));
    _handles.push_back(std::move(handle));
  }

  template <typename... Args>
  void emit(EventType tag, Args&&... args) {
    _dispatcher.emit(tag, std::forward<Args>(args)...);
  }

  void clearListeners() {
    for (auto& h : _handles) h.unsubscribe();
    _handles.clear();
  }

  ~Entity() {
    clearListeners();
  }

 private:
  Dispatcher& _dispatcher;
  std::vector<Handle> _handles;
};