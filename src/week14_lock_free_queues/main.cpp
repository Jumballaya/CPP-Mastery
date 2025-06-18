#include <iostream>

#include "async/LockFreeQueue.hpp"

int main() {
  LockFreeQueue<int> q(8);

  bool ok = q.try_enqueue(42);

  if (ok) {
    std::cout << "Passed" << std::endl;
  }

  return 0;
}