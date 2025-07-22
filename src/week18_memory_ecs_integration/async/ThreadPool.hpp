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
  void enqueue(Fn&& fn);
  void enqueue(Job<>&& job);
  void waitForIdle();

 private:
  void start(std::size_t count);
  void stop();

  std::vector<std::thread> _threads;
  std::vector<std::unique_ptr<LockFreeQueue<Job<>>>> _queues;
  std::atomic<bool> _shutdown{false};
  std::atomic<size_t> _activeJobs{0};

  size_t getLeastLoadedQueue() const;
  bool trySteal(size_t theifId, Job<>& job);
};

template <typename Fn>
void ThreadPool::enqueue(Fn&& fn) {
  _activeJobs.fetch_add(1, std::memory_order_relaxed);
  Job job;
  job.set(std::forward<Fn>(fn));
  size_t idx = getLeastLoadedQueue();
  while (!_queues[idx]->try_enqueue(std::move(job))) {
    std::this_thread::yield();
  }
}