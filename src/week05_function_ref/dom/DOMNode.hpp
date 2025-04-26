#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "../EventBus.hpp"
#include "../EventDispatcher.hpp"
#include "../traits.hpp"
#include "EventEmitter.hpp"
#include "dom_types.hpp"

using DOMEventEmitter = ApplyTuple<DOMFns>::apply<EventEmitter, DOMEvents>;
using DOMDispatcher = ApplyTuple<DOMFns>::apply<EventDispatcher, DOMEvents>;

class DOMNode : public DOMEventEmitter, public std::enable_shared_from_this<DOMNode> {
 public:
  DOMNode(std::shared_ptr<DOMDispatcher> dispatcher) : DOMEventEmitter(dispatcher) {}
  ~DOMNode() = default;

  void appendChild(std::shared_ptr<DOMNode> child) {
    _children.push_back(child);
    child->_parent = shared_from_this();
  }

  void setColor(float r, float g, float b) {
    _color[0] = r;
    _color[1] = g;
    _color[2] = b;
    emit(DOMEvents::Change, r, g, b);
  }

  void setPosition(float x, float y) {
    _position[0] = x;
    _position[1] = y;
    emit(DOMEvents::Move, x, y);
  }

  void setSize(int w, int h) {
    _size[0] = w;
    _size[1] = h;
    emit(DOMEvents::Resize, w, h);
  }

 private:
  std::vector<std::shared_ptr<DOMNode>> _children;
  std::weak_ptr<DOMNode> _parent;

  float _color[3] = {0.0f, 0.0f, 0.0f};
  float _position[2] = {0.0f, 0.0f};
  int _size[2] = {0, 0};
};