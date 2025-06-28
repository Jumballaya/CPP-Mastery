#pragma once

#include "ecs/World.hpp"
#include "ecs/component/Component.hpp"
#include "ecs/system/System.hpp"
#include "ecs/system/SystemScheduler.hpp"
#include "ecs/system/SystemTraits.hpp"

namespace Tag {
struct PositionUpdate {};
struct RenderUpdate {};
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

class MovementSystem : public System {
 public:
  void update(World& world, float dt) override {
    for (auto [id, pos, vel] : world.view<Position, Velocity>()) {
      pos->x += vel->dx * dt;
      pos->y += vel->dy * dt;
    }
  }
  const char* name() const override { return "MovementSystem"; }
};

template <>
struct SystemTraits<MovementSystem> {
  using Reads = TypeList<Position>;
  using Writes = TypeList<Velocity>;
  using Provides = TypeList<Tag::PositionUpdate>;
  using DependsOn = EmptyList;
};

struct RenderState : public Component<RenderState> {
  COMPONENT_NAME("RenderState");
  RenderState(bool v) : visible(v) {}
  bool visible = true;
};

class RenderSystem : public System {
 public:
  void update(World& world, float dt) override {
    for (auto [id, pos, rs] : world.view<Position, RenderState>()) {
      if (pos->x >= 8.0) {
        rs->visible = false;
      }
      std::cout << "Rendering Entity " << id.index
                << " at (" << pos->x << ", " << pos->y << ")\n";
    }
  }

  const char* name() const override { return "RenderSystem"; }
};

template <>
struct SystemTraits<RenderSystem> {
  using Reads = TypeList<Position>;
  using Writes = TypeList<RenderState>;
  using DependsOn = TypeList<Tag::PositionUpdate>;
  using Provides = TypeList<>;
};