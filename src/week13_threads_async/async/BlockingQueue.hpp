#pragma once

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <optional>
#include <queue>
#include <utility>

template <typename T>
class BlockingQueue {
 public:
  BlockingQueue() = default;
  ~BlockingQueue() {
    invalidate();
  }

  BlockingQueue(const BlockingQueue&) = delete;
  BlockingQueue& operator=(const BlockingQueue&) = delete;

  BlockingQueue(BlockingQueue&& other) noexcept {
    std::scoped_lock lock(other._mutex);
    _queue = std::move(other._queue);
    _valid = _valid && other._valid;
    other._valid = false;
  }

  BlockingQueue& operator=(BlockingQueue&& other) noexcept {
    if (this != &other) {
      std::scoped_lock lock(_mutex, other._mutex);
      _queue = std::move(other._queue);
      _valid = _valid && other._valid;
      other._valid = false;
    }
    return *this;
  }

  void push(T item) {
    {
      std::unique_lock lock(_mutex);
      _queue.push(std::move(item));
    }
    _cv.notify_one();
  }

  std::optional<T> pop() {
    std::unique_lock lock(_mutex);
    _cv.wait(lock, [&] { return !_queue.empty() || !_valid; });

    if (!_valid && _queue.empty()) {
      return std::nullopt;
    }

    T item = std::move(_queue.front());
    _queue.pop();
    return item;
  }

  std::optional<T> try_pop() {
    std::unique_lock lock(_mutex);
    if (_queue.empty()) return std::nullopt;
    T item = std::move(_queue.front());
    _queue.pop();
    return item;
  }

  void invalidate() {
    {
      std::unique_lock lock(_mutex);
      _valid = false;
    }
    _cv.notify_all();
  }

  void swap(BlockingQueue& other) noexcept {
    std::scoped_lock lock(_mutex, other._mutex);
    std::swap(_queue, other._queue);
    std::swap(_valid, other._valid);
  }

  size_t size() const {
    std::unique_lock lock(_mutex);
    return _queue.size();
  }

  bool isValid() const noexcept {
    return _valid;
  }

 private:
  std::queue<T> _queue;
  std::mutex _mutex;
  std::condition_variable _cv;
  bool _valid = true;
};