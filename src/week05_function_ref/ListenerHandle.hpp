#pragma once

#include <cstdint>

#include "EventBus.hpp"

using ListenerID = uint32_t;

template <typename EventType, typename... Fns>
class ListenerHandle {
 public:
  ListenerHandle(EventBus<EventType, Fns...>* bus, EventType tag, ListenerID id)
      : _bus(bus), _tag(tag), _id(id) {}

  ~ListenerHandle() {
    if (!_active || _bus == nullptr) return;
    _bus->unsubscribe(_tag, _id);
    _active = false;
    _bus = nullptr;
    _id = 0;
  }

  ListenerHandle(const ListenerHandle&) noexcept = delete;
  ListenerHandle& operator=(const ListenerHandle&) noexcept = delete;

  bool unsubscribe() {
    if (!_active || _bus == nullptr) return false;

    bool res = _bus->unsubscribe(_tag, _id);
    _active = false;
    _id = 0;
    return res;
  }

  EventType tag() const { return _tag; }
  ListenerID id() const { return _id; }
  bool active() const { return _active; }

 private:
  EventBus<EventType, Fns...>* _bus;
  EventType _tag;
  ListenerID _id;
  bool _active = true;
};