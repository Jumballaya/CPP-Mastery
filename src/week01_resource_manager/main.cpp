#include <iostream>
#include <memory>
#include <string>

#include "ResourceManager.hpp"
#include "ScopedLogger.hpp"

#define LOGGING_ENABLED false

int main() {
  ScopedLogger log_main("Main Block", LOGGING_ENABLED);

  ScopedLogger log_resource_building("Resource Creation", LOGGING_ENABLED);
  ResourceManager<std::string> manager;
  manager.load("greeting", []() { return std::make_shared<std::string>("Hello World\n"); });
  manager.load("new-greeting", []() { return std::make_shared<std::string>("Hello Galaxy\n"); });
  manager.unload("greeting");
  log_resource_building.end();

  manager.clear();

  ScopedLogger log_resource_display("Resource Display", LOGGING_ENABLED);
  auto val = manager.get("greeting");
  if (!val) {
    val = manager.get("new-greeting");
    if (!val) {
      std::cout << "Greeting not found" << std::endl;
      return 0;
    }
  }
  log_resource_display.end();

  std::cout << *val;
}