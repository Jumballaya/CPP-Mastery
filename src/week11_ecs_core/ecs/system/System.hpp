#pragma once

class World;

class System {
 public:
  virtual ~System() = default;
  virtual void update(World& world, float dt) = 0;
  virtual const char* name() const { return "UNNAMED_SYSTEM"; }
};