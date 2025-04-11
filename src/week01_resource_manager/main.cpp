#include <iostream>
#include <memory>
#include <string>

#include "MoveOnlyBuffer.hpp"
#include "MoveOnlyResourceManager.hpp"
#include "ResourceManager.hpp"
#include "ScopedGuard.hpp"
#include "ScopedLogger.hpp"
#include "ScopedTimer.hpp"

#define LOGGING_ENABLED true

void demo_1_resource_manager() {
  ResourceManager<std::string> manager;
  manager.load("greeting", []() { return std::make_shared<std::string>("Hello World\n"); });
  manager.load("new-greeting", []() { return std::make_shared<std::string>("Hello Galaxy\n"); });
  manager.unload("greeting");

  auto val = manager.get("greeting");
  if (!val) {
    val = manager.get("new-greeting");
    if (!val) {
      std::cout << "Greeting not found" << std::endl;
      return;
    }
  }

  std::cout << *val;
}

void demo_2_scoped_logger() {
  ScopedLogger log_main("Main Block", LOGGING_ENABLED);
  ScopedLogger log_resource_building("Resource Creation", LOGGING_ENABLED);
  ResourceManager<std::string> manager;
  manager.load("greeting", []() { return std::make_shared<std::string>("Hello World\n"); });
  manager.load("new-greeting", []() { return std::make_shared<std::string>("Hello Galaxy\n"); });
  manager.unload("greeting");
  log_resource_building.end();
  ScopedLogger log_resource_display("Resource Display", LOGGING_ENABLED);
  auto val = manager.get("greeting");
  if (!val) {
    val = manager.get("new-greeting");
    if (!val) {
      std::cout << "Greeting not found" << std::endl;
      return;
    }
  }
  log_resource_display.end();

  std::cout << *val;
}

void demo_3_scoped_guard() {
  ScopedLogger log_main("Main Block", LOGGING_ENABLED);

  ScopedLogger log_resource_building("Resource Creation", LOGGING_ENABLED);
  ResourceManager<std::string> manager;
  manager.load("greeting", []() { return std::make_shared<std::string>("Hello World\n"); });
  manager.load("new-greeting", []() { return std::make_shared<std::string>("Hello Galaxy\n"); });
  manager.unload("greeting");
  log_resource_building.end();

  ScopedGuard defer([&]() {
    std::cout << "Clearing Manager" << std::endl;
    manager.clear();
  });

  ScopedLogger log_resource_display("Resource Display", LOGGING_ENABLED);
  auto val = manager.get("greeting");
  if (!val) {
    val = manager.get("new-greeting");
    if (!val) {
      std::cout << "Greeting not found" << std::endl;
      return;
    }
  }
  log_resource_display.end();

  std::cout << *val;
}

void demo_4_scoped_timer() {
  ScopedTimer timer("Demo 4", LOGGING_ENABLED);
  demo_3_scoped_guard();
  for (int i = 0; i < 100000000; i++) {
    std::cout << "";
  }
}

void demo_5_move_only_resource_manager() {
  MoveOnlyResourceManager<std::unique_ptr<std::string>> manager;
  manager.load("greeting", std::make_unique<std::string>("Hello World\n"));

  auto* val = manager.get("greeting");
  if (val) std::cout << **val;
}

void demo_6_move_only_buffer() {
  MoveOnlyResourceManager<MoveOnlyBuffer> manager;
  MoveOnlyBuffer buffer(6);

  buffer.write(0, 'H');
  buffer.write(1, 'E');
  buffer.write(2, 'L');
  buffer.write(3, 'L');
  buffer.write(4, 'O');
  buffer.write(5, '\n');

  manager.load("greeting", std::move(buffer));

  const MoveOnlyBuffer* result = manager.get("greeting");
  if (!result) {
    std::cout << "Greeting not found" << std::endl;
    return;
  }

  for (size_t i = 0; i < 6; i++) {
    std::cout << result->read(i);
  }
  std::cout << std::endl;
}

int main() {
  demo_6_move_only_buffer();
}
