#pragma once

#include <memory>

#include "../EventDispatcher.hpp"
#include "../traits.hpp"
#include "DOMNode.hpp"
#include "EventEmitter.hpp"
#include "dom_types.hpp"

using DOMDispatcher = ApplyTuple<DOMFns>::apply<EventDispatcher, DOMEvents>;

class DOM {
 public:
  DOM() {
    _dispatcher = std::make_shared<DOMDispatcher>();
    _root = std::make_shared<DOMNode>(_dispatcher);
  }

  void dispatch() { _dispatcher->dispatch_now(); }
  std::shared_ptr<DOMNode> root() const { return _root; }

  std::shared_ptr<DOMNode> createNode() {
    auto node = std::make_shared<DOMNode>(_dispatcher);
    return node;
  }

 private:
  std::shared_ptr<DOMNode> _root;
  std::shared_ptr<DOMDispatcher> _dispatcher;
};