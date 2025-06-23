#pragma once

#include <atomic>
#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../async/Job.hpp"
#include "../async/LockFreeQueue.hpp"
#include "../async/ThreadPool.hpp"
#include "TaskId.hpp"

class TaskBuilder;

class TaskGraph {
 public:
  TaskGraph(size_t workerCount = 4);
  ~TaskGraph() = default;

  template <typename Fn>
  TaskId addTask(Fn&& func);
  void addDependency(TaskId dependent, TaskId prerequisite);
  void execute();

 private:
  struct TaskNode {
    Job<> job;
    std::atomic<int> dependencyCount{0};
    std::vector<TaskId> dependents;
  };

  std::unordered_map<TaskId, TaskNode> _tasks;

  std::atomic<size_t> _nextId{0};
  ThreadPool _threadPool;

  void onTaskComplete(TaskId id);

  TaskId generateTaskId();
};

template <typename Fn>
TaskId TaskGraph::addTask(Fn&& func) {
  TaskId taskId = generateTaskId();

  Job job;
  job.set(std::forward<Fn>(func));

  TaskNode node;
  node.job = std::move(job);

  _tasks.try_emplace(
      taskId,
      std::move(node.job),
      node.dependencyCount.load(),
      std::move(node.dependents));

  return taskId;
}