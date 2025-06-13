#pragma once
#include <chrono>
#include <future>
#include <utility>

class AsyncTaskBase {
 public:
  virtual void run() = 0;
  virtual ~AsyncTaskBase() = default;
};

template <typename T>
class AsyncTask : public AsyncTaskBase {
 public:
  template <typename Fn>
  explicit AsyncTask(Fn&& fn)
      : _task(std::forward<Fn>(fn)), _future(_task.get_future()) {}

  void run() override {
    if (_task.valid()) _task();
  }

  std::future<T> takeFuture() {
    return std::move(_future);
  }

 private:
  std::packaged_task<T()> _task;
  std::future<T> _future;
};

template <>
class AsyncTask<void> : public AsyncTaskBase {
 public:
  template <typename Fn>
  explicit AsyncTask(Fn&& fn)
      : _task(std::forward<Fn>(fn)), _future(_task.get_future()) {}

  void run() override {
    if (_task.valid()) _task();
  }

  std::future<void> takeFuture() {
    return std::move(_future);
  }

 private:
  std::packaged_task<void()> _task;
  std::future<void> _future;
};