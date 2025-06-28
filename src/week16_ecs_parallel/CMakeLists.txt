add_subdirectory(async)
add_subdirectory(tasks)
add_subdirectory(ecs)

add_executable(ecs_parallel
main.cpp
demo.hpp
)

target_link_libraries(ecs_parallel
  async
  tasks
  ecs
)