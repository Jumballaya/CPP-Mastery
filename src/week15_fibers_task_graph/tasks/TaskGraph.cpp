#include "TaskGraph.hpp"

TaskGraph::TaskGraph(size_t workerCount) : _threadPool(workerCount, 1024) {}

void TaskGraph::addDependency(TaskId dependent, TaskId prerequisite) {
  auto& depNode = _tasks.at(dependent);
  auto& prereqNode = _tasks.at(prerequisite);
  depNode.dependencyCount.fetch_add(1, std::memory_order_relaxed);
  prereqNode.dependents.push_back(dependent);
}

void TaskGraph::execute() {
  for (auto& [id, node] : _tasks) {
    if (node.dependencyCount.load(std::memory_order_relaxed) == 0) {
      Job job = node.job;
      _threadPool.enqueue([this, id, job]() {
        job();
        onTaskComplete(id);
      });
    }
  }
}

TaskId TaskGraph::generateTaskId() {
  return TaskId{_nextId++};
}

void TaskGraph::onTaskComplete(TaskId id) {
  auto& node = _tasks.at(id);

  for (TaskId dependentId : node.dependents) {
    auto& dependentNode = _tasks.at(dependentId);
    Job job = dependentNode.job;
    _threadPool.enqueue([this, dependentId, job]() {
      job();
      onTaskComplete(dependentId);
    });
  }
}
