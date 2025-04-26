#pragma once

template <typename NamespaceTag, typename Events>
class DispatchToken {
 public:
  explicit DispatchToken(Events tag) : _tag(tag) {}

  Events tag() const { return _tag; }

 private:
  Events _tag;
};