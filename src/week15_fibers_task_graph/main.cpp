#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <thread>

#include "tasks/JobSystem.hpp"
#include "tasks/TaskGraph.hpp"

void demo_1_task_graph_basic() {
  unsigned int threadCount = std::max(1u, std::thread::hardware_concurrency() / 2);
  JobSystem job((size_t)threadCount);

  int count = 0;
  while (true) {
    if (count > 10) break;

    TaskGraph graph;

    TaskId a = graph.addTask([] { std::cout << "Task A running" << std::endl; });
    TaskId b = graph.addTask([] { std::cout << "Task B running" << std::endl; });
    TaskId c = graph.addTask([] { std::cout << "Task C running" << std::endl; });

    graph.addDependency(b, a);
    graph.addDependency(c, b);

    job.execute(graph);
    count++;
  }
}

struct Message {
  uint32_t id;
  uint32_t gen;
  uint32_t copy = 0;
  std::string msg;

  Message() = default;

  ~Message() {
    std::cout << "Message [destroyed]: " << *this << std::endl;
  }

  Message(const Message& other) {
    id = other.id;
    msg = other.msg;
    gen = other.gen;
    copy = other.copy + 1;
    std::cout << "Message [copied ctor]: " << other << std::endl;
  }

  Message& operator=(const Message& other) {
    if (this == &other) return *this;

    id = other.id;
    msg = other.msg;
    copy = other.copy + 1;
    std::cout << "Message [copied assign]: " << other << std::endl;

    return *this;
  }

  friend std::ostream& operator<<(std::ostream& os, const Message& message) {
    os << message.msg << " | ID: " << message.id << " | Gen: " << message.gen << " | Copies: " << message.copy;
    return os;
  }
};

void demo_2_task_graph_scope_data() {
  int id = 0;
  unsigned int threadCount = std::max(1u, std::thread::hardware_concurrency() / 2);
  JobSystem job(threadCount);

  for (int i = 0; i < 5; ++i) {
    TaskGraph graph;

    Message messageA;
    messageA.id = id++;
    messageA.gen = i;
    messageA.msg = "Task A running";

    Message messageB;
    messageB.id = id++;
    messageB.gen = i;
    messageB.msg = "Task B running";

    Message messageC;
    messageC.id = id++;
    messageC.gen = i;
    messageC.msg = "Task C running";

    TaskId a = graph.addTask([&messageA] { std::cout << messageA << std::endl; });
    TaskId b = graph.addTask([&messageB] { std::cout << messageB << std::endl; });
    TaskId c = graph.addTask([&messageC] { std::cout << messageC << std::endl; });

    graph.addDependency(b, a);
    graph.addDependency(c, b);

    job.execute(graph);
    job.waitForCompletion();
  }

  std::cout << "Thread Count: " << threadCount << std::endl;
}

int main() {
  demo_2_task_graph_scope_data();
}