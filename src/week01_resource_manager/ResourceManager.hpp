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

  std::shared_ptr<T> load(const std::string &name, std::function<std::shared_ptr<T>()> loader)
  {
    auto found = resources_.find(name);
    if (found != resources_.end())
    {
      return found->second;
    }
    auto loaded = loader();
    resources_[name] = loaded;
    return loaded;
  }

  std::shared_ptr<T> get(const std::string &name) const
  {
    auto found = resources_.find(name);
    if (found == resources_.end())
    {
      return std::shared_ptr<T>{};
    }
    return found->second;
  };

  void unload(const std::string &name) {};

  void clear() {};

private:
  std::unordered_map<std::string, std::shared_ptr<T>> resources_;
};