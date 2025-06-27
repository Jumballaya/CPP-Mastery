#pragma once

#include <tuple>

#include "component/ComponentStorage.hpp"
#include "entity/EntityId.hpp"

template <typename... Ts>
class View {
  using PrimaryT = std::tuple_element_t<0, std::tuple<Ts...>>;
  using StorageTuple = std::tuple<ComponentStorage<Ts>&...>;

  class Iterator {
   public:
    using PrimaryIterator = typename ComponentStorage<PrimaryT>::MapType::iterator;

    Iterator(PrimaryIterator current, PrimaryIterator end, StorageTuple& storages)
        : _current(current), _end(end), _storages(&storages) {
      advanceToNextValid();
    }

    Iterator& operator++() {
      ++_current;
      advanceToNextValid();
      return *this;
    }

    auto operator->() const = delete;

    std::tuple<EntityId, Ts*...> operator*() const {
      EntityId id = _current->first;
      return std::tuple_cat(std::make_tuple(id),
                            getComponentsFor<Ts...>(id, *_storages, std::index_sequence_for<Ts...>{}));
    }

    bool operator==(const Iterator& other) const {
      return _current == other._current;
    }

    bool operator!=(const Iterator& other) const {
      return !(*this == other);
    }

   private:
    PrimaryIterator _current;
    PrimaryIterator _end;
    StorageTuple* _storages;

    void advanceToNextValid() {
      while (_current != _end) {
        EntityId id = _current->first;
        bool allPresent = std::apply(
            [&](auto&... storages) {
              // Usage of the C++17 fold expression. This is a binary left fold
              // e.g. storages = (x, y, z), this: (... && storages.has(id))
              // is converted to this: x.has(id) && y.has(id) && z.has(id)
              return (... && storages.has(id));
            },
            *_storages);
        if (allPresent) break;
        ++_current;
      }
    }

    template <typename... Us, std::size_t... Is>
    static std::tuple<Us*...> getComponentsFor(
        EntityId id,
        const std::tuple<ComponentStorage<Us>&...>& storages,
        std::index_sequence<Is...>) {
      return std::make_tuple(std::get<Is>(storages).get(id)...);
    }
  };

 public:
  View(ComponentStorage<Ts>&... storages) : _storages(storages...) {}

  ~View() = default;
  View(const View&) = delete;
  View& operator=(const View&) = delete;
  View(View&&) = default;
  View& operator=(View&&) = default;

  Iterator begin() {
    auto& primary = std::get<0>(_storages);
    return Iterator{primary.begin(), primary.end(), _storages};
  }

  Iterator end() {
    auto& primary = std::get<0>(_storages);
    return Iterator{primary.end(), primary.end(), _storages};
  }

  size_t size() const;
  bool empty() const;

 private:
  StorageTuple _storages;
};