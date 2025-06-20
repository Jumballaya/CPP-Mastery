add_executable(lock_free_queues main.cpp
  async/ThreadPool.cpp
  assets/AssetPipeline.cpp)

# Copy assets to build directory on build
add_custom_command(
  TARGET lock_free_queues POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/files
          $<TARGET_FILE_DIR:lock_free_queues>/files
)