#pragma once

#include <cstddef>
#include <cstdint>

#include "MessageBus.hpp"

using ListenerID = uint32_t;

template <typename EventType, size_t N, typename... Fns>
class ListenerHandle {
 public:
  ListenerHandle(MessageBus<EventType, N, Fns...>* bus, EventType tag, ListenerID id)
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

  ListenerHandle(ListenerHandle&& other) noexcept
      : _bus(other._bus), _tag(other._tag), _id(other._id), _active(other._active) {
    other._bus = nullptr;
    other._id = 0;
    other._active = false;
  }

  ListenerHandle& operator=(ListenerHandle&& other) noexcept {
    _bus = std::move(other._bus);
    _tag = other._tag;
    _id = other._id;
    _active = other._active;
    other._bus = nullptr;
    other._id = 0;
    other._active = false;
  }

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
  MessageBus<EventType, N, Fns...>* _bus;
  EventType _tag;
  ListenerID _id;
  bool _active = true;
};