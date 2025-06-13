#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

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

int main() {
  std::mutex mut;
  WorkGroup group;

  for (int i = 0; i < 10; i++) {
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