#pragma once

#include <string_view>

#include "./TrackedObject.hpp"
#include "./TrackedPayload.hpp"

class TrackedComponent : public TrackedObject {
 public:
  TrackedComponent() : TrackedObject("default"), _payload("default payload", 0) {}
  TrackedComponent(std::string_view name, TrackedPayload payload) : TrackedObject(name), _payload(std::move(payload)) {}
  TrackedComponent(std::string_view name, std::string_view obj_name, size_t obj_size) : TrackedObject(name), _payload(obj_name, obj_size) {}

  TrackedComponent(const TrackedComponent& other) noexcept : TrackedObject(other._name), _payload(other._payload) {}
  TrackedComponent(TrackedComponent&& other) noexcept : TrackedObject(std::move(other._name)), _payload(std::move(other._payload)) {}

  TrackedComponent& operator=(const TrackedComponent& other) {
    TrackedObject::operator=(other);
    if (this == &other) return *this;
    _payload = other._payload;
    return *this;
  }

  TrackedComponent& operator=(TrackedComponent&& other) {
    TrackedObject::operator=(other);
    if (this == &other) return *this;
    _payload = std::move(other._payload);
    return *this;
  }

  const TrackedPayload get() const {
    return _payload;
  }

 private:
  TrackedPayload _payload;
};