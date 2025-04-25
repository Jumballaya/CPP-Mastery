#pragma once

#include <cstdint>

#include "VariantCallback.hpp"

enum class EventTag : uint32_t {
  Log,
  Damage,
  Click
};

template <typename... Signatures>
class EventBus {
 public:
  EventBus() = default;
  ~EventBus() = default;

  using Listener = VariantCallback<Signatures...>;
  using ListenerID = uint32_t;

  template <typename Callable>
  ListenerID subscribe(EventTag tag, Callable& cb);

  template <typename... Args>
  void publish(EventTag tag, Args&&... args);

  bool unsubscribe(EventTag tag, ListenerID id);

  void clear(EventTag tag);

  void clearAll();

  size_t listenerCount(EventTag tag) const;

 private:
};