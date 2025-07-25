#include "ecs/World.hpp"
#include "ecs/component/Component.hpp"
#include "ecs/entity/EntityId.hpp"
#include "ecs/system/System.hpp"

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
    for (auto [id, pos] : world.view<Position>()) {
      std::cout << "Entity " << id.index << " => (" << pos->x << ", " << pos->y << ")\n";
    }
  }
}

int main() {
  demo_1_simple_ecs();
}