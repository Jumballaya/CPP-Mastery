#include <iostream>
#include <thread>
#include <utility>

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

int main() {
  Work w([]() {
    std::cout << "Running on a new thread!" << std::endl;
  });

  std::cout << "Main thread!" << std::endl;
}