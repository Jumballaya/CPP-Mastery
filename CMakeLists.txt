cmake_minimum_required(VERSION 3.16)
project(cpp_learning)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)


add_compile_options(-fconcepts)
add_compile_options(-msse3)

add_subdirectory(src/week01_resource_manager)
add_subdirectory(src/week02_tracked_object)
add_subdirectory(src/week03_scoped_transaction)
add_subdirectory(src/week04_slice_view)
add_subdirectory(src/week05_function_ref)
add_subdirectory(src/week06_allocator_selector)
add_subdirectory(src/week07_custom_containers)
add_subdirectory(src/week08_stl_allocator_integration)
add_subdirectory(src/week09_crtp)
add_subdirectory(src/week10_templates_simd)
add_subdirectory(src/week11_ecs_core)
add_subdirectory(src/week12_concepts_constraints)
add_subdirectory(src/week13_threads_async)