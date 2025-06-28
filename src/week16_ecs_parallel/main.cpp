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
  world.registerComponent<RenderState>();
  world.registerSystem<MovementSystem>();
  world.registerSystem<RenderSystem>();

  for (int i = 0; i < 10; ++i) {
    EntityId id = world.createEntity();
    world.addComponent<Position>(id, float(i), float(i));
    world.addComponent<Velocity>(id, 1.0f, 0.5f);
    world.addComponent<RenderState>(id, true);
  }

  for (int frame = 0; frame < 5; ++frame) {
    std::cout << "Frame " << frame << ":\n";
    jobs.beginFrame();
    TaskGraph graph;
    world.buildExecutionGraph(graph, 1.0f);
    jobs.execute(graph);
    jobs.endFrame();
    for (auto [id, pos, rs] : world.view<Position, RenderState>()) {
      std::cout << "Entity " << id.index << " => (" << pos->x << ", " << pos->y << ")" << ", visible: " << (rs->visible ? "true" : "false") << "\n";
    }
  }
}