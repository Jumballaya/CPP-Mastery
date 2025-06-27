#include "JobSystem.hpp"

JobSystem::JobSystem(size_t workerCount) : _threadPool(workerCount, 1024) {}

void JobSystem::execute(TaskGraph& graph) {
  bool workRemaning = true;

  while (workRemaning) {
    auto readyJobs = graph.fetchReadyJobs();
    for (auto& [id, job] : readyJobs) {
      submit(std::move(job));
    }
    waitForCompletion();
    workRemaning = !graph.isComplete();
  }
}

void JobSystem::submit(Job<>&& job) {
  _threadPool.enqueue(std::move(job));
}

void JobSystem::waitForCompletion() {
  _threadPool.waitForIdle();
}

void JobSystem::beginFrame() {
  // eventually this is for frame allocators, and any top of the
  // frame stuff and for any frame-local stuff
}

void JobSystem::endFrame() {
  waitForCompletion();
  // eventually for flushing any frame-local stuff
}