#include "ecs/World.hpp"
#include "ecs/component/Component.hpp"
#include "ecs/entity/EntityId.hpp"
#include "ecs/system/System.hpp"

struct Position : public Component<Position> {
  static constexpr std::string_view typeName = "Position";
  Position(float _x, float _y) : x(_x), y(_y) {}
  float x = 0;
  float y = 0;
};

struct Velocity : public Component<Velocity> {
  static constexpr std::string_view typeName = "Velocity";
  Velocity(float _dx, float _dy) : dx(_dx), dy(_dy) {}
  float dx = 0;
  float dy = 0;
};

class MovementSystem : public System {
 public:
  void update(World& world, float dt) override {
    world.eachEntity([&](EntityId id) {
      auto* pos = world.getComponent<Position>(id);
      auto* vel = world.getComponent<Velocity>(id);
      if (pos && vel) {
        pos->x += vel->dx * dt;
        pos->y += vel->dy * dt;
      }
    });
  }

  const char* name() const override { return "MovementSystem"; }
};

void demo_1_simple_ecs() {
  World world;

  // Create 10 test entities
  for (int i = 0; i < 10; ++i) {
    EntityId id = world.createEntity();
    world.addComponent<Position>(id, float(i), float(i));
    world.addComponent<Velocity>(id, 1.0f, 0.5f);
  }

  // Register and run system
  world.registerSystem<MovementSystem>();

  for (int frame = 0; frame < 5; ++frame) {
    std::cout << "Frame " << frame << ":\n";
    world.update(1.0f);
    world.eachEntity([&](EntityId id) {
      auto* pos = world.getComponent<Position>(id);
      if (pos) {
        std::cout << "Entity " << id.index << " => (" << pos->x << ", " << pos->y << ")\n";
      }
    });
  }
}

int main() {
  demo_1_simple_ecs();
}