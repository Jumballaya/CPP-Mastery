cmake_minimum_required(VERSION 3.16)
project(cpp_learning)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src/week01_resource_manager)
add_subdirectory(src/week02_tracked_object)
add_subdirectory(src/week03_scoped_transaction)
add_subdirectory(src/week04_slice_view)