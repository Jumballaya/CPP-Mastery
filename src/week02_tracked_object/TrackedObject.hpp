#pragma once
#include <iostream>
#include <string>
#include <string_view>

struct TrackedObject {
  TrackedObject(std::string_view name) : _name(name) {
    std::cout << "[ " << name << " ] : Constructed" << std::endl;
    ++constructed;
  };
  virtual ~TrackedObject() {
    std::cout << "[ " << _name << " ] : Deleted" << std::endl;
    ++destructed;
  };

  // Copy/Assign Copy
  TrackedObject(const TrackedObject& other) {
    std::cout << "[ " << other._name << " ] : Copied to : [" << _name << " ]" << std::endl;
    ++copied;
  };
  TrackedObject& operator=(const TrackedObject& other) {
    if (this != &other) {
      std::cout << "[ " << other._name << " ] : Copied to New : [" << _name << " ]" << std::endl;
      _name = "copy_assigned_" + other._name;
      ++copy_assigned;
    }
    return *this;
  };

  // Move/Assign Move
  TrackedObject(TrackedObject&& other) noexcept {
    std::cout << "[ " << other._name << " ] : Moved to : [" << _name << " ]" << std::endl;
    ++moved;
  };
  TrackedObject& operator=(TrackedObject&& other) noexcept {
    if (this != &other) {
      std::cout << "[ " << other._name << " ] : Moved to New : [" << _name << " ]" << std::endl;
      _name = std::move(other._name);
      ++move_assigned;
    }
    return *this;
  };

  const std::string& name() const {
    return _name;
  };

  static void print_stats() {
    std::cout << "\n--- TrackedObject Stats ---\n";
    std::cout << "Constructed:    " << constructed << "\n";
    std::cout << "Destructed:     " << destructed << "\n";
    std::cout << "Copied:         " << copied << "\n";
    std::cout << "Copy Assigned:  " << copy_assigned << "\n";
    std::cout << "Moved:          " << moved << "\n";
    std::cout << "Move Assigned:  " << move_assigned << "\n";
    std::cout << "----------------------------\n";
  }

 protected:
  std::string _name;

 private:
  inline static int constructed = 0;
  inline static int destructed = 0;
  inline static int copied = 0;
  inline static int copy_assigned = 0;
  inline static int moved = 0;
  inline static int move_assigned = 0;
};