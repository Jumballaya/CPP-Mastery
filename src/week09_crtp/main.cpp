#include <iostream>

#include "ComponentRegistry.hpp"

struct Transform : Component<Transform> {
  float translation[3];
  float rotation[4];
  float scale[3];
};
struct Velocity : Component<Velocity> {
  float x;
  float y;
  float z;
};
struct Health : Component<Health> {
  float amount;
};

void demo_1_registry() {
  ComponentRegistry registry;
  registry.registerComponent<Transform>("Transform");
  registry.registerComponent<Velocity>("Velocity");
  registry.registerComponent<Health>("Health");

  auto trans = registry.getInfo(Transform::typeId());
  if (trans) {
    std::cout << "Component: " << trans->name << ", size: " << trans->size << "\n";
  }

  auto vel = registry.getInfo(Velocity::typeId());
  if (vel) {
    std::cout << "Component: " << vel->name << ", size: " << vel->size << "\n";
  }

  auto health = registry.getInfo(Health::typeId());
  if (health) {
    std::cout << "Component: " << health->name << ", size: " << health->size << "\n";
  }
}

int main() {
  demo_1_registry();
}