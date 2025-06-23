#include <chrono>
#include <iostream>
#include <thread>

#include "tasks/TaskGraph.hpp"

void demo_1_task_graph_basic() {
  TaskGraph graph;

  TaskId a = graph.addTask([] { std::cout << "Task A running" << std::endl; });
  TaskId b = graph.addTask([] { std::cout << "Task B running" << std::endl; });
  TaskId c = graph.addTask([] { std::cout << "Task C running" << std::endl; });

  graph.addDependency(b, a);
  graph.addDependency(c, b);

  graph.execute();

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int main() {
  demo_1_task_graph_basic();
}