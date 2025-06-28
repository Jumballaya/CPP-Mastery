#include <iostream>

#include "demo.hpp"
#include "ecs/World.hpp"
#include "tasks/JobSystem.hpp"
#include "tasks/TaskGraph.hpp"

int main() {
  JobSystem jobs;
  World world;

  world.registerComponent<Position>();
  world.registerComponent<Velocity>();
  world.registerComponent<Acceleration>();
  world.registerComponent<Health>();
  world.registerComponent<RenderState>();

  world.registerSystem<InputSystem>();
  world.registerSystem<AccelerationSystem>();
  world.registerSystem<PhysicsSystem>();
  world.registerSystem<RenderSystem>();
  world.registerSystem<DamageSystem>();

  for (int i = 0; i < 10; ++i) {
    EntityId id = world.createEntity();
    world.addComponent<Position>(id, float(i), float(i));
    world.addComponent<Velocity>(id, 1.0f, 0.5f);
    world.addComponent<Acceleration>(id, 0.2f, 0.1f);
    world.addComponent<Health>(id, 100.0f);
    world.addComponent<RenderState>(id, true);
  }

  for (int frame = 0; frame < 5; ++frame) {
    std::cout << "Frame " << frame << ":\n";

    jobs.beginFrame();
    TaskGraph graph;
    world.buildExecutionGraph(graph, 1.0f);
    jobs.execute(graph);
    jobs.endFrame();

    for (auto [id, pos, vel, acc, health, rs] : world.view<Position, Velocity, Acceleration, Health, RenderState>()) {
      std::cout << "Entity " << id.index
                << " => Pos(" << pos->x << ", " << pos->y << ")"
                << " Vel(" << vel->dx << ", " << vel->dy << ")"
                << " Acc(" << acc->ax << ", " << acc->ay << ")"
                << " Health: " << health->current
                << " Visible: " << (rs->visible ? "true" : "false") << "\n";
    }
  }
}