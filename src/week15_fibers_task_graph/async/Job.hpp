#pragma once

struct Job {
  void (*func)(void*);
  void* data;

  void operator()() const {
    func(data);
  }
};