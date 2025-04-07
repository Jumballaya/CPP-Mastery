#include <iostream>
#include <memory>
#include <string>

#include "ResourceManager.hpp"

int main() {
  ResourceManager<std::string> manager;
  manager.load("greeting", []() { return std::make_shared<std::string>("Hello World\n"); });
  manager.load("new-greeting", []() { return std::make_shared<std::string>("Hello Galaxy\n"); });
  manager.unload("greeting");

  manager.clear();

  auto val = manager.get("greeting");
  if (!val) {
    val = manager.get("new-greeting");
    if (!val) {
      std::cout << "Greeting not found" << std::endl;
      return 0;
    }
  }

  std::cout << *val;
}