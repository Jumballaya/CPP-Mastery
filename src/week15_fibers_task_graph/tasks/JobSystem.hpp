#pragma once

#include <atomic>
#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../async/Job.hpp"
#include "../async/LockFreeQueue.hpp"
#include "../async/ThreadPool.hpp"
#include "TaskGraph.hpp"
#include "TaskId.hpp"

class JobSystem {
 public:
  JobSystem(size_t workerCount = 4);
  ~JobSystem() = default;

  void execute(TaskGraph& graph);
  void submit(Job<>&& job);
  void waitForCompletion();

 private:
  ThreadPool _threadPool;
};