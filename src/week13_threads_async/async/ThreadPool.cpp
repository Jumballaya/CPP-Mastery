#include "ThreadPool.hpp"

ThreadPool::ThreadPool(std::size_t count) {
  start(count);
}

ThreadPool::~ThreadPool() {
  stop();
}

void ThreadPool::start(std::size_t count) {
  for (std::size_t i = 0; i < count; ++i) {
    _threads.emplace_back([this] {
      while (true) {
        auto task = _queue.pop();
        if (!task) break;
        task->run();
      }
    });
  }
}

void ThreadPool::stop() {
  _shutdown.store(true);
  _queue.invalidate();
  for (auto& t : _threads) {
    if (t.joinable()) t.join();
  }
  _threads.clear();
}