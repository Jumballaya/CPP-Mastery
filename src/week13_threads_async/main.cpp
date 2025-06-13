#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include "async/BlockingQueue.hpp"

class Work {
 public:
  template <typename Fn>
  explicit Work(Fn&& fn) : t(std::forward<Fn>(fn)) {}

  ~Work() {
    if (t.joinable()) t.join();
  }

  Work(const Work&) = delete;
  Work& operator=(const Work&) = delete;

  Work(Work&& other) noexcept : t(std::move(other.t)) {}
  Work& operator=(Work&& other) noexcept {
    if (&other == this) return *this;
    if (t.joinable()) t.join();
    t = std::move(other.t);
    return *this;
  }

  bool isRunning() const { return t.joinable(); }

 private:
  std::thread t;
};

class WorkGroup {
 public:
  WorkGroup() = default;
  ~WorkGroup() {
    if (_joinable) {
      join();
    }
  };
  WorkGroup(const WorkGroup&) = delete;
  WorkGroup& operator=(const WorkGroup&) = delete;
  WorkGroup(WorkGroup&&) = delete;
  WorkGroup& operator=(WorkGroup&&) = delete;

  template <typename Fn>
  void addWork(Fn&& fn) {
    Work w(std::forward<Fn>(fn));
    _group.push_back(std::move(w));
  }

  void join() {
    _joinable = false;
    _group.clear();
  }

  bool joinable() const {
    return _joinable;
  }

 private:
  std::vector<Work> _group;
  bool _joinable = true;
};

void demo_1_threads() {
  Work w([]() {
    std::cout << "Inside the new thread" << std::endl;
  });

  std::cout << "Main thread!" << std::endl;
}

void demo_1_mutex() {
  const int workerCount = 10;
  std::mutex mut;
  WorkGroup group;
  for (int i = 0; i < 10; ++i) {
    group.addWork([&mut, i]() {
      {
        std::unique_lock lock(mut);
        std::cout << "Work " << i << " started\n";
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      {
        std::unique_lock lock(mut);
        std::cout << "Work " << i << " finished\n";
      }
    });
  }
}

void demo_3_blocking_queue() {
  const int workerCount = 4;
  BlockingQueue<std::function<void()>> queue;
  std::mutex mut;
  WorkGroup group;

  for (int i = 0; i < workerCount; ++i) {
    group.addWork([&queue, &mut, i]() {
      while (true) {
        auto task = queue.pop();
        if (!task.has_value()) {
          std::unique_lock lock(mut);
          std::cout << "Worker " << i << " exiting\n";
          break;
        }

        {
          std::unique_lock lock(mut);
          std::cout << "Worker " << i << " executing task\n";
        }

        (*task)();
      }
    });
  }

  for (int i = 0; i < 10; ++i) {
    queue.push([i]() {
      std::cout << "Task " << i << " is running\n";
    });
  }

  queue.invalidate();
}

void demo_4_thread_pool() {
}

int main() {
  demo_3_blocking_queue();
}