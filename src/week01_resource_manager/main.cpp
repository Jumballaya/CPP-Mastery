#include <iostream>
#include <string>
#include <memory>
#include "ResourceManager.hpp"

int main()
{
  ResourceManager<std::string> manager;

  manager.load("greeting", []()
               { return std::make_shared<std::string>("Hello World\n"); });

  auto val = manager.get("greeting");
  if (!val)
  {
    std::cout << "Greeting not found" << std::endl;
    return 0;
  }

  std::cout << *manager.get("greeting");
}