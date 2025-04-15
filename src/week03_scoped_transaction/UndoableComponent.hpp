#pragma once

template <typename T>
class UndoableComponent {
 public:
  UndoableComponent(const T& wrapped) : _committed(wrapped), _working(wrapped), _rollback(wrapped), _dirty(false) {}
  ~UndoableComponent() = default;

  UndoableComponent(const UndoableComponent& other) : _committed(other._committed),
                                                      _working(other._working),
                                                      _rollback(other._rollback),
                                                      _dirty(other._dirty) {}

  UndoableComponent(UndoableComponent&& other) : _committed(std::move(other._committed)),
                                                 _working(std::move(other._working)),
                                                 _rollback(std::move(other._rollback)),
                                                 _dirty(other._dirty) {}

  UndoableComponent& operator=(const UndoableComponent& other) {
    if (this == &other) return *this;
    _committed = other._committed;
    _working = other._working;
    _rollback = other._rollback;
    _dirty = other._dirty;
    return *this;
  }

  UndoableComponent& operator=(UndoableComponent&& other) {
    if (this == &other) return *this;
    _committed = std::move(other._committed);
    _working = std::move(other._working);
    _rollback = std::move(other._rollback);
    _dirty = other._dirty;
    return *this;
  }

  void commit() {
    _rollback = _committed;
    _committed = _working;
    _dirty = false;
  }

  void undo() {
    _working = _committed;
    _committed = _rollback;
    _dirty = false;
  }

  T& edit() {
    _dirty = true;
    return _working;
  }

  bool is_dirty() const {
    return _dirty;
  }

  const T& current() const {
    return _working;
  }

  const T& value() const {
    return _committed;
  }

 private:
  T _working;    // working copy
  T _committed;  // committed/final copy
  T _rollback;   // last _commited before the current one

  bool _dirty = false;
};