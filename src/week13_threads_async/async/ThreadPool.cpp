#include "ThreadPool.hpp"

ThreadPool::ThreadPool(std::size_t threadCount) {
  start(threadCount);
}

ThreadPool::~ThreadPool() {
  stop();
}

ThreadPool::ThreadPool(ThreadPool&& other) noexcept {
  *this = std::move(other);
}

ThreadPool& ThreadPool::operator=(ThreadPool&& other) noexcept {
  if (this != &other) {
    stop();
    _queue = std::move(other._queue);
    _workers = std::move(other._workers);
    _shutdown.store(other._shutdown.load());
    other._shutdown = true;
  }
  return *this;
}

void ThreadPool::start(std::size_t threadCount) {
  for (std::size_t i = 0; i < threadCount; ++i) {
    _workers.emplace_back([this, i] {
      while (true) {
        auto task = _queue.pop();
        if (!task.has_value()) break;
        (*task)();
      }
    });
  }
}

void ThreadPool::stop() {
  if (_shutdown.exchange(true)) return;
  _queue.invalidate();
  for (auto& t : _workers) {
    if (t.joinable()) t.join();
  }
  _workers.clear();
}

void ThreadPool::enqueue(std::function<void()> task) {
  _queue.push(std::move(task));
}