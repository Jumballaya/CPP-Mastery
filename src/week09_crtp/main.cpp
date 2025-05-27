#include <iostream>

#include "ComponentRegistry.hpp"
#include "ComponentStorage.hpp"
#include "EntityId.hpp"

struct Transform : Component<Transform> {
  static constexpr std::string_view typeName = "Transform";
  float translation[3];
  float rotation[4];
  float scale[3];
};
struct Velocity : Component<Velocity> {
  static constexpr std::string_view typeName = "Velocity";
  float x;
  float y;
  float z;
};
struct Health : Component<Health> {
  static constexpr std::string_view typeName = "Health";
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

void demo_2_storage() {
  ComponentRegistry registry;
  registry.registerComponent<Transform>("Transform");
  registry.registerComponent<Velocity>("Velocity");
  registry.registerComponent<Health>("Health");

  EntityId e0 = 0;
  EntityId e1 = 1;
  EntityId e2 = 2;

  ComponentStorage<Transform> transStore;
  ComponentStorage<Velocity> velStore;
  ComponentStorage<Health> healthStore;

  transStore.insert(e0, Transform{
                            .translation = {0.0f, 0.0f, 0.0f},
                            .rotation = {3.14f, 0.0f, 0.0f, 0.0f},
                            .scale = {0.0f, 0.0f, 0.0f},
                        });

  velStore.insert(e1, Velocity{.x = 6.123f, .y = 0.0f, .z = 0.0f});
  healthStore.insert(e2, Health{.amount = 101.3f});

  if (transStore.has(e0)) {
    auto trans = transStore.get(e0);
    std::cout << "Component: " << Transform::name() << ", size: " << Transform::sizeBytes() << "\n"
              << "rotation.x: " << trans->rotation[0] << std::endl;
  }

  if (transStore.has(e1)) {
    std::cout << "Error!!! transStore does not have e1!!!" << std::endl;
  }

  if (velStore.has(e1)) {
    auto vel = velStore.get(e1);
    std::cout << "Component: " << Velocity::name() << ", size: " << Velocity::sizeBytes() << "\n"
              << "x: " << vel->x << std::endl;
  }

  if (velStore.has(e2)) {
    std::cout << "Error!!! velStore does not have e2!!!" << std::endl;
  }

  if (healthStore.has(e2)) {
    auto hp = healthStore.get(e2);
    std::cout << "Component: " << Health::name() << ", size: " << Health::sizeBytes() << "\n"
              << "amount: " << hp->amount << std::endl;
  }

  if (healthStore.has(e0)) {
    std::cout << "Error!!! healthStore does not have e0!!!" << std::endl;
  }
}

int main() {
  demo_2_storage();
}