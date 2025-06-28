#pragma once

#include "ecs/World.hpp"
#include "ecs/component/Component.hpp"
#include "ecs/system/System.hpp"
#include "ecs/system/SystemScheduler.hpp"
#include "ecs/system/SystemTraits.hpp"

//
//
//    InputSystem
//       ├──> PhysicsSystem ─┬──> RenderSystem
//       │                   └──> DamageSystem
//       └──> AccelerationSystem
//
//

namespace Tag {
struct InputUpdate {};
struct PhysicsUpdate {};
};  // namespace Tag

struct Position : public Component<Position> {
  COMPONENT_NAME("Position");
  Position(float _x, float _y) : x(_x), y(_y) {}
  float x = 0;
  float y = 0;
};

struct Velocity : public Component<Velocity> {
  COMPONENT_NAME("Velocity");
  Velocity(float _dx, float _dy) : dx(_dx), dy(_dy) {}
  float dx = 0;
  float dy = 0;
};

struct Acceleration : public Component<Acceleration> {
  COMPONENT_NAME("Acceleration");
  Acceleration(float _ax, float _ay) : ax(_ax), ay(_ay) {}
  float ax = 0;
  float ay = 0;
};

struct RenderState : public Component<RenderState> {
  COMPONENT_NAME("RenderState");
  RenderState(bool v) : visible(v) {}
  bool visible = true;
};

struct Health : public Component<Health> {
  COMPONENT_NAME("Health");
  Health(float c) : current(c) {}
  float current = 100.0f;
};

class InputSystem : public System {
 public:
  void update(World& world, float dt) override {
    for (auto [id, vel] : world.view<Velocity>()) {
      vel->dx += 0.1f * dt;  // Fake input
      vel->dy += 0.1f * dt;
    }
  }
  const char* name() const override { return "InputSystem"; }
};

template <>
struct SystemTraits<InputSystem> {
  using Reads = TypeList<>;
  using Writes = TypeList<Velocity>;
  using DependsOn = TypeList<>;
  using Provides = TypeList<Tag::InputUpdate>;
};

class AccelerationSystem : public System {
 public:
  void update(World& world, float dt) override {
    for (auto [id, acc, vel] : world.view<Acceleration, Velocity>()) {
      vel->dx += acc->ax * dt;
      vel->dy += acc->ay * dt;
    }
  }
  const char* name() const override { return "AccelerationSystem"; }
};

template <>
struct SystemTraits<AccelerationSystem> {
  using Reads = TypeList<Acceleration>;
  using Writes = TypeList<Velocity>;
  using DependsOn = TypeList<Tag::InputUpdate>;
  using Provides = TypeList<>;
};

class PhysicsSystem : public System {
 public:
  void update(World& world, float dt) override {
    for (auto [id, pos, vel] : world.view<Position, Velocity>()) {
      pos->x += vel->dx * dt;
      pos->y += vel->dy * dt;
    }
  }
  const char* name() const override { return "PhysicsSystem"; }
};

template <>
struct SystemTraits<PhysicsSystem> {
  using Reads = TypeList<Velocity>;
  using Writes = TypeList<Position>;
  using DependsOn = TypeList<Tag::InputUpdate>;
  using Provides = TypeList<Tag::PhysicsUpdate>;
};

class RenderSystem : public System {
 public:
  void update(World& world, float dt) override {
    for (auto [id, pos, rs] : world.view<Position, RenderState>()) {
      rs->visible = (pos->x >= 0 && pos->y >= 0);  // Simple visibility logic
    }
  }
  const char* name() const override { return "RenderSystem"; }
};

template <>
struct SystemTraits<RenderSystem> {
  using Reads = TypeList<Position>;
  using Writes = TypeList<RenderState>;
  using DependsOn = TypeList<Tag::PhysicsUpdate>;
  using Provides = TypeList<>;
};

class DamageSystem : public System {
 public:
  void update(World& world, float dt) override {
    for (auto [id, pos, health] : world.view<Position, Health>()) {
      if (pos->x > 5.0f) {
        health->current -= 1.0f * dt;  // Apply damage if entity moves past a point
      }
    }
  }
  const char* name() const override { return "DamageSystem"; }
};

template <>
struct SystemTraits<DamageSystem> {
  using Reads = TypeList<Position>;
  using Writes = TypeList<Health>;
  using DependsOn = TypeList<Tag::PhysicsUpdate>;
  using Provides = TypeList<>;
};