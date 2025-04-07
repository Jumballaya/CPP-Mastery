#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <functional>

template <typename T>
class ResourceManager
{
public:
  ResourceManager() = default;
  std::shared_ptr<T> load(const std::string &name, std::function<std::shared_ptr<T>()> loader);
  std::shared_ptr<T> get(const std::string &name) const;
  void unload(const std::string &name);
  void clear();

private:
  std::unordered_map<std::string, std::shared_ptr<T>> resources_;
};