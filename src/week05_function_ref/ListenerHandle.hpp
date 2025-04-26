#pragma once

#include <cstdint>

#include "EventBus.hpp"

using ListenerID = uint32_t;

template <typename... Fns>
class ListenerHandle {
 public:
  ListenerHandle(EventBus<Fns...>* bus, EventTag tag, ListenerID id)
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

  EventTag tag() const { return _tag; }
  ListenerID id() const { return _id; }
  bool active() const { return _active; }

 private:
  EventBus<Fns...>* _bus;
  EventTag _tag;
  ListenerID _id;
  bool _active = true;
};