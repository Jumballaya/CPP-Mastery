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
    _activeJobs.fetch_add(1, std::memory_order_relaxed);
    Job job;
    job.set(std::forward<Fn>(fn));
    while (!_queue.try_enqueue(std::move(job))) {
      std::this_thread::yield();
    }
  }

  void enqueue(Job<>&& job) {
    _activeJobs.fetch_add(1, std::memory_order_relaxed);
    while (!_queue.try_enqueue(std::move(job))) {
      std::this_thread::yield();
    }
  }

  void waitForIdle() {
    while (_activeJobs.load(std::memory_order_acquire) > 0) {
      std::this_thread::yield();
    }
  }

 private:
  void start(std::size_t count);
  void stop();

  std::vector<std::thread> _threads;
  LockFreeQueue<Job<>> _queue;
  std::atomic<bool> _shutdown{false};
  std::atomic<size_t> _activeJobs{0};
};