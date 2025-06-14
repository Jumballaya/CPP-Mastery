#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>

#include "async/BlockingQueue.hpp"
#include "async/ThreadPool.hpp"
#include "http/HTTPServer.hpp"
#include "net/Command.hpp"
#include "net/TcpGameClient.hpp"
#include "net/TcpGameServer.hpp"

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
  ThreadPool pool(4);

  std::mutex printMutex;

  for (int i = 0; i < 10; ++i) {
    pool.enqueue([i, &printMutex]() {
      {
        std::unique_lock lock(printMutex);
        std::cout << "Task " << i << " is running on thread " << std::this_thread::get_id() << "\n";
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    });
  }

  std::this_thread::sleep_for(std::chrono::seconds(2));  // let tasks run
}

void demo_4_futures() {
  std::packaged_task<int()> step1([] {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    throw std::runtime_error("Panic!!");
    return 21;
  });
  std::future<int> f1 = step1.get_future();
  std::thread t1(std::move(step1));

  std::packaged_task<int()> step2([&f1] {
    try {
      int x = f1.get();
      return x * 2;
    } catch (const std::exception& ex) {
      std::cout << "Caught exception from f1: " << ex.what() << std::endl;
    }
    return 0;
  });
  std::future<int> f2 = step2.get_future();
  std::thread t2(std::move(step2));

  std::packaged_task<int()> step3([&f2] {
    try {
      int x = f2.get();
      return x + 1337;
    } catch (const std::exception& ex) {
      std::cout << "Caught exception from f2: " << ex.what() << std::endl;
    }
    return 0;
  });
  std::future<int> f3 = step3.get_future();
  std::thread t3(std::move(step3));

  std::cout << "Final result: " << f3.get() << std::endl;

  t1.join();
  t2.join();
  t3.join();
}

void demo_5_thread_pool() {
  ThreadPool pool(4);

  auto f1 = pool.submit([] {
    return 42;
  });

  auto f2 = pool.submit([] {
    return std::string("hello");
  });

  auto f3 = pool.submit([] {
    std::cout << "Task running!\n";
  });

  std::cout << "f1: " << f1.get() << "\n";
  std::cout << "f2: " << f2.get() << "\n";
  f3.get();
}

void demo_6_http_server() {
  ThreadPool pool(4);
  HTTPServer server(pool);
  server.listen(8080, [] {
    std::cout << "Listening on port 8080" << std::endl;
  });

  std::this_thread::sleep_until(std::chrono::system_clock::time_point::max());
  server.stop();
}

void runClient() {
  TcpGameClient client("127.0.0.1", 3000);

  client.onMessage([](std::string_view msg) {
    std::cout << "Server replied: " << msg << "\n";
  });

  if (!client.connect()) {
    std::cerr << "Failed to connect to server.\n";
    return;
  }

  client.send("chat:send?str:from=person&str:text=hello world\n");
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

void demo_7_tcp_client_server() {
  ThreadPool pool(4);
  TcpGameServer server(pool);
  server.route("chat:send", [](TcpClient& client, const Command& cmd) {
    std::string from = cmd.get<std::string>("from");
    std::string text = cmd.get<std::string>("text");
    std::cout << from << ": " << text << "\n";
    client.send("Received!\n");
  });

  server.listen(3000, [] {
    std::cout << "Server listening on port 3000...\n";
  });

  runClient();
}

int main() {
  demo_7_tcp_client_server();
}
