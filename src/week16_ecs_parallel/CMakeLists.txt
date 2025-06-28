add_executable(ecs_parallel main.cpp
  async/ThreadPool.cpp
  tasks/TaskGraph.cpp
  tasks/JobSystem.cpp
  ecs/World.cpp
  ecs/entity/EntityBuilder.cpp
  ecs/entity/EntityRef.cpp
  ecs/system/SystemScheduler.cpp
)