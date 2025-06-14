#pragma once

#include <cstddef>
#include <format>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

class CommandValue {
 public:
  using CommandVariant = std::variant<std::monostate, int, float, bool, std::string>;

  CommandValue() = default;
  explicit CommandValue(std::string name, CommandVariant value)
      : _name(std::move(name)), _value(std::move(value)) {}

  static CommandValue from(const std::string& type, const std::string& valueStr) {
    if (type == "int") {
      try {
        return CommandValue("", std::stoi(valueStr));
      } catch (...) {
        throw std::runtime_error("Invalid int: " + valueStr);
      }
    } else if (type == "float") {
      try {
        return CommandValue("", std::stof(valueStr));
      } catch (...) {
        throw std::runtime_error("Invalid float: " + valueStr);
      }
    } else if (type == "bool") {
      if (valueStr == "true") return CommandValue("", true);
      if (valueStr == "false") return CommandValue("", false);
      throw std::runtime_error("Invalid bool: " + valueStr);
    } else if (type == "str") {
      return CommandValue("", valueStr);
    } else {
      throw std::runtime_error("Unknown type: " + type);
    }
  }

  const std::string& name() const { return _name; }

  template <typename T>
  T get() const {
    if (auto val = std::get_if<T>(&_value)) return *val;
    throw std::runtime_error("CommandValue type mismatch for key: " + _name);
  }

  template <typename T>
  bool has() const {
    return std::holds_alternative<T>(_value);
  }

  bool isNull() const {
    return std::holds_alternative<std::monostate>(_value);
  }

 private:
  std::string _name;
  CommandVariant _value;
};

class Command {
 public:
  Command() = default;
  ~Command() = default;

  static Command parse(std::string_view input) {
    Command cmd;

    auto questionPos = input.find('?');
    cmd._path = std::string(input.substr(0, questionPos));

    if (questionPos == std::string_view::npos) return cmd;

    std::string_view query = input.substr(questionPos + 1);
    while (!query.empty()) {
      auto ampPos = query.find('&');
      auto pair = query.substr(0, ampPos);

      auto eq = pair.find('=');
      if (eq != std::string_view::npos) {
        auto key = pair.substr(0, eq);   // e.g. "str:from"
        auto val = pair.substr(eq + 1);  // e.g. "user"

        auto colon = key.find(':');
        std::string type, name;
        if (colon != std::string_view::npos) {
          type = std::string(key.substr(0, colon));   // "str" of "str:from"
          name = std::string(key.substr(colon + 1));  // "from" of "str:from"
        } else {
          type = "str";
          name = std::string(key);
        }
        cmd._entries[name] = CommandValue::from(type, std::string(val));
      }
      if (ampPos == std::string_view::npos) break;
      query.remove_prefix(ampPos + 1);
    }

    return cmd;
  }

  const std::string& path() const { return _path; }

  template <typename T>
  T get(const std::string& key) const {
    auto it = _entries.find(key);
    if (it == _entries.end()) {
      throw std::runtime_error("Missing key: " + key);
    }
    return it->second.get<T>();
  }

  template <typename T>
  bool has(const std::string& key) const {
    auto it = _entries.find(key);
    return it != _entries.end() && it->second.has<T>();
  }

 private:
  std::string _path;
  std::unordered_map<std::string, CommandValue> _entries;

  static std::string urlDecode(std::string_view str) {
    std::string out;
    for (size_t i = 0; i < str.size(); ++i) {
      if (str[i] == '%' && i + 2 < str.size()) {
        std::string hex = {str[i + 1], str[i + 2]};
        out += static_cast<char>(std::stoi(hex, nullptr, 16));
        i += 2;
      } else if (str[i] == '+') {
        out += ' ';
      } else {
        out += str[i];
      }
    }
    return out;
  }

  static CommandValue::CommandVariant parseValue(const std::string& type, const std::string& value) {
    try {
      if (type == "int") return std::stoi(value);
      if (type == "float") return std::stof(value);
      if (type == "bool") {
        if (value == "true") return true;
        if (value == "false") return false;
        throw std::runtime_error("Invalid bool value: " + value);
      }
      return value;
    } catch (...) {
      throw std::runtime_error("Failed to parse value: '" + value + "' as type: " + type);
    }
  }
};