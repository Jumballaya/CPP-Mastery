#pragma once
#include <atomic>
#include <functional>
#include <future>
#include <memory>
#include <thread>
#include <vector>

#include "TaskQueue.hpp"

class ThreadPool {
 public:
  explicit ThreadPool(std::size_t count);
  ~ThreadPool();

  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;

  template <typename Fn>
  auto submit(Fn&& fn) -> std::future<std::invoke_result_t<Fn>> {
    using ReturnT = std::invoke_result_t<Fn>;

    auto task = std::make_unique<AsyncTask<ReturnT>>(std::forward<Fn>(fn));
    auto future = task->takeFuture();

    _queue.push(std::move(task));
    return future;
  }

  template <typename Fn>
  void enqueue(Fn&& fn) {
    using ReturnT = std::invoke_result_t<Fn>;
    auto task = std::make_unique<AsyncTask<ReturnT>>(std::forward<Fn>(fn));
    _queue.push(std::move(task));
  }

 private:
  void start(std::size_t count);
  void stop();

  std::vector<std::thread> _threads;
  TaskQueue _queue;
  std::atomic<bool> _shutdown{false};
};