#include "TaskGraph.hpp"

void TaskGraph::addDependency(TaskId dependent, TaskId prerequisite) {
  auto& depNode = _tasks.at(dependent);
  auto& prereqNode = _tasks.at(prerequisite);
  depNode.remainingDependencies.fetch_add(1, std::memory_order_relaxed);
  prereqNode.dependents.push_back(dependent);
}

std::vector<std::pair<TaskId, Job<>>> TaskGraph::fetchReadyJobs() {
  std::vector<std::pair<TaskId, Job<>>> ready;
  for (auto& [id, node] : _tasks) {
    if (node.remainingDependencies.load(std::memory_order_relaxed) == 0 && !node.submitted) {
      node.submitted = true;
      ready.emplace_back(id, std::move(node.job));
    }
  }
  return ready;
}

TaskId TaskGraph::generateTaskId() {
  return TaskId{_nextId++};
}

bool TaskGraph::isComplete() const {
  return _remainingTasks.load(std::memory_order_acquire) == 0;
}

void TaskGraph::onTaskComplete(TaskId id) {
  auto& node = _tasks.at(id);

  for (TaskId dependentId : node.dependents) {
    auto& depNode = _tasks.at(dependentId);
    depNode.remainingDependencies.fetch_sub(1, std::memory_order_acq_rel);
  }

  _remainingTasks.fetch_sub(1, std::memory_order_release);
}