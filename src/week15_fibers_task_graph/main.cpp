#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>

#include "tasks/TaskGraph.hpp"

void demo_1_task_graph_basic() {
  int count = 0;
  while (true) {
    if (count > 10) break;

    TaskGraph graph;

    TaskId a = graph.addTask([] { std::cout << "Task A running" << std::endl; });
    TaskId b = graph.addTask([] { std::cout << "Task B running" << std::endl; });
    TaskId c = graph.addTask([] { std::cout << "Task C running" << std::endl; });

    graph.addDependency(b, a);
    graph.addDependency(c, b);

    graph.execute();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    count++;
  }
}

struct Message {
  uint32_t id;
  std::string msg;

  Message() = default;

  ~Message() {
    std::cout << "Message: " << id << " destroyed" << std::endl;
  }

  Message(const Message& other) {
    id = other.id + 1;
    msg = other.msg;
    std::cout << "Message copied [ctor]: " << other.id << std::endl;
  }

  Message& operator=(const Message& other) {
    if (this == &other) return *this;

    id = other.id + 1;
    msg = other.msg;
    std::cout << "Message copied [assign]: " << other.id << std::endl;

    return *this;
  }

  friend std::ostream& operator<<(std::ostream& os, const Message& message) {
    os << message.msg << ": " << message.id;
    return os;
  }
};

void demo_2_task_graph_scope_data() {
  int count = 0;
  while (true) {
    if (count > 2) break;

    TaskGraph graph;

    Message message;
    message.id = 0;
    message.msg = "From Main Thread";

    TaskId a = graph.addTask([message] { std::cout << "Task A running: " << message << std::endl; });
    TaskId b = graph.addTask([message] { std::cout << "Task B running: " << message << std::endl; });
    TaskId c = graph.addTask([message] { std::cout << "Task C running: " << message << std::endl; });

    graph.addDependency(b, a);
    graph.addDependency(c, b);

    graph.execute();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    count++;
  }
}

int main() {
  demo_2_task_graph_scope_data();
}