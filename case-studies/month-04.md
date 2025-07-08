# Case Study – Month 4

## AI Based Learning

The learning process is getting smoother, but ChatGPT is getting worse at keeping up. On any substantial system like the ECS it gets lost and tends to loop back on itself if it can't solve a problem or bug. This has been great practice for me, though, I have been able to get a lot of debugging time in on my own. Its language is always so positive when I tell it that some suggestion it made was terrible and caused a bug, but it is quick to make sure to not include the bug again, mostly.

It will often put itself in this weird Sisyphean state where it will rock between 2 terrible designs over several prompts. You show it an error: It will suggest a fix, then when prompted with the next error it will slowly begin to work its way back to your original code. You will show it the error from that code and the cycle begins again. When I hit these points, I just abandon ChatGPT and look for other sources but it is usually in the middle of trying to build some type of thing that I have never done before so it just feels like getting left in the ocean sometimes.

As frustrating as it might be, I am almost done so I will just keep truckin'

**Here is the month's case study:**

## Multithreading and Job System Design

This month was huge! Learning about multithreading in C++ was great and it helped me learn some things about how multithreading and async work in other languages like Rust and Go. I had used multithreaded programming in other languages, including locks/mutex/atomics/etc. but I learned a whole lot more fine-grained control was possible. Specifically with memory access ordering, like acquire/release/relaxed, it was new to me and really cool to learn about.

I also learned about MPMC, lock-free, and wait-free structures including queues and double-ended queues. I watched GDC videos on job systems, multithreading and fibers that inspired a lot of what I ended up building. I didn't build a coroutine or fiber system, but I would love to in the future at some point.

At the start of the month I learned about the thread header and how to work with thread, how to pool threads and how to build a blocking queue with mutexes that many threads could use. This approach was basic, but usable enough to build small HTTPServer and TCPServer classes to showcase multithreaded applications.

```cpp
class HTTPServer {
 public:
  HTTPServer(ThreadPool& pool);
  ~HTTPServer();
  void listen(int port, std::function<void()> onListen);
  void stop();

 private:
  int _serverFd;
  std::thread _acceptThread;
  ThreadPool& _pool;
  std::atomic<bool> _shutdown;
};
```

I then learned about what lock-free and wait-free meant and built a lock-free queue that the thread pool could use to queue up work. I built a multithreaded asset pipeline class that could be used to upload assets using an asset manager. It allowed for parallel loading and a progress bar.

```cpp
class AssetPipeline {
 public:
  AssetPipeline(AssetManager& manager, ThreadPool& threadpool);

  void scanDirectory(const std::filesystem::path& dir);
  void loadAllAsync();

  size_t totalFiles() const;
  size_t loadedFiles() const;
  float progress() const;

  AssetHandle getHandle(const std::string& path) const;
  const std::vector<std::string>& error() const { return _errors; }

 private:
  void enqueueLoadJob(const std::filesystem::path& path);

  AssetManager& _manager;
  ThreadPool& _threadPool;

  std::vector<std::filesystem::path> _files;
  std::unordered_map<std::string, AssetHandle> _pathToHandle;
  std::vector<std::string> _errors;

  std::atomic<size_t> _loadedCount{0};
  std::atomic<size_t> _totalCount{0};
  std::atomic<size_t> _submittedCount{0};
};
```

After that was learning about fibers, coroutines and job systems. I did not implement fibers or coroutines, but I have used coroutines in other languages and watched some videos on fibers. Below is fromn the header of the `JobSystem` that I built. It is extremely simple and just built around the thread pool.

```cpp
class JobSystem {
 public:
  JobSystem(size_t workerCount = 4);
  ~JobSystem() = default;

  void execute(TaskGraph& graph);
  void submit(Job<>&& job);
  void waitForCompletion();

  void beginFrame();
  void endFrame();

 private:
  ThreadPool _threadPool;
};
```

The last month was dedicated to upgrading the ECS and getting it useable. It was finally able to build a dependency graph and then fire off multithreaded jobs for each system, based on its dependents like the following:

```cpp
//    InputSystem
//       ├──> PhysicsSystem ─┬──> RenderSystem
//       │                   └──> DamageSystem
//       └──> AccelerationSystem
```

Overall, it was an extremely busy month and I am now filled with a bunch of new knowledge to practice and master.
