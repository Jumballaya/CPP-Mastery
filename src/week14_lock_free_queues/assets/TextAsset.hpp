#pragma once
#include <string>
#include <typeindex>
#include <typeinfo>

#include "AssetBase.hpp"

class TextAsset : public AssetBase {
 public:
  TextAsset(std::string path, std::string contents)
      : _filePath(std::move(path)), _contents(std::move(contents)) {}

  const std::string& contents() const { return _contents; }
  const std::string& filePath() const { return _filePath; }

  std::type_index type() const override { return typeid(TextAsset); }

 private:
  std::string _filePath;
  std::string _contents;
};