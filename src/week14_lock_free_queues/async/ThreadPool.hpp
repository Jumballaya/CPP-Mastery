#pragma once
#include <atomic>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include "./Job.hpp"
#include "./LockFreeQueue.hpp"

class ThreadPool {
 public:
  explicit ThreadPool(std::size_t count, std::size_t queueCapacity);
  ~ThreadPool();

  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;

  template <typename Fn>
  auto submit(Fn&& fn) -> std::future<std::invoke_result_t<Fn>> {
    using ReturnT = std::invoke_result_t<Fn>;
    auto* task = new std::packaged_task<ReturnT()>(std::forward<Fn>(fn));
    std::future<ReturnT> future = task->get_future();

    Job job;
    job.func = [](void* ptr) {
      auto* task = static_cast<std::packaged_task<ReturnT()>*>(ptr);
      (*task)();
      delete task;
    };
    job.data = static_cast<void*>(task);

    while (!_queue.try_enqueue(std::move(job))) {
      // retry if the queue is full
      std::this_thread::yield();
    }

    return future;
  }

  template <typename Fn>
  void enqueue(Fn&& fn) {
    using ReturnT = std::invoke_result_t<Fn>;
    auto* task = new std::packaged_task<ReturnT()>(std::forward<Fn>(fn));

    Job job;
    job.func = [](void* ptr) {
      auto* task = static_cast<std::packaged_task<ReturnT()>*>(ptr);
      (*task)();
      delete task;
    };
    job.data = static_cast<void*>(task);

    while (!_queue.try_enqueue(std::move(job))) {
      std::this_thread::yield();
    }
  }

 private:
  void start(std::size_t count);
  void stop();

  std::vector<std::thread> _threads;
  LockFreeQueue<Job> _queue;
  std::atomic<bool> _shutdown{false};
};