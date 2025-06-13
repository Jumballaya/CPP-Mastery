#pragma once
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

#include "AsyncTask.hpp"

class TaskQueue {
 public:
  void push(std::unique_ptr<AsyncTaskBase> task) {
    {
      std::lock_guard lock(_mutex);
      _queue.push(std::move(task));
    }
    _cv.notify_one();
  }

  std::unique_ptr<AsyncTaskBase> pop() {
    std::unique_lock lock(_mutex);
    _cv.wait(lock, [&] { return !_queue.empty() || !_valid; });

    if (!_valid && _queue.empty())
      return nullptr;

    auto task = std::move(_queue.front());
    _queue.pop();
    return task;
  }

  void invalidate() {
    {
      std::lock_guard lock(_mutex);
      _valid = false;
    }
    _cv.notify_all();
  }

 private:
  std::queue<std::unique_ptr<AsyncTaskBase>> _queue;
  std::mutex _mutex;
  std::condition_variable _cv;
  bool _valid = true;
};