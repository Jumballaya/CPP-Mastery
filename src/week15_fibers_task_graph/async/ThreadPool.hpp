#pragma once
#include <atomic>
#include <cstddef>
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
  void enqueue(Fn&& fn) {
    Job job;
    job.set(std::forward<Fn>(fn));
    while (!_queue.try_enqueue(std::move(job))) {
      std::this_thread::yield();
    }
  }

  template <size_t MaxSize = 64>
  void enqueue(Job<MaxSize>&& job) {
    while (!_queue.try_enqueue(std::move(job))) {
      std::this_thread::yield();
    }
  }

 private:
  void start(std::size_t count);
  void stop();

  std::vector<std::thread> _threads;
  LockFreeQueue<Job<>> _queue;
  std::atomic<bool> _shutdown{false};
};