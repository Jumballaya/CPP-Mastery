include(FetchContent)

# nlohmann_json
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)

FetchContent_MakeAvailable(nlohmann_json)


add_executable(memory_ecs
  main.cpp

  async/ThreadPool.cpp

  ecs/entity/EntityBuilder.cpp
  ecs/entity/EntityRef.cpp
  ecs/system/SystemScheduler.cpp

  ecs/World.cpp

  memory/AllocatorTagRegistry.cpp
  memory/FrameArena.cpp
  memory/LinearAllocator.cpp
  memory/ThreadArenaRegistry.cpp

  tasks/JobSystem.cpp
  tasks/TaskGraph.cpp
)

target_link_libraries(memory_ecs PUBLIC nlohmann_json::nlohmann_json)