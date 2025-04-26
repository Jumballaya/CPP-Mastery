#pragma once

#include "event_tags.hpp"

template <typename NamespaceTag>
class DispatchToken {
 public:
  explicit DispatchToken(EventTag tag) : _tag(tag) {}

  EventTag tag() const { return _tag; }

 private:
  EventTag _tag;
};