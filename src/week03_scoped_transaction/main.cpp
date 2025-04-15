#include <iostream>

#include "../week02_tracked_object/TrackedPayload.hpp"
#include "ScopedTransaction.hpp"
#include "UndoableComponent.hpp"

void demo_1_scoped_transaction() {
  TrackedPayload health("HP", 100);
  std::cout << "Before transaction: " << health.size() << std::endl;

  {
    ScopedTransaction<TrackedPayload> tx(health);
    tx.get().resize(200);  // modify the working copy
    tx.commit();           // apply it
  }

  std::cout << "After commit: " << health.size() << std::endl;

  {
    ScopedTransaction<TrackedPayload> tx(health);
    tx.get().resize(50);  // modify again
    // no commit
  }

  std::cout << "After rollback: " << health.size() << std::endl;

  TrackedObject::print_stats();
}

void demo_2_transaction_exception() {
  TrackedPayload health("HP", 50);
  std::cout << "Initial Size: " << health.size() << std::endl;

  try {
    ScopedTransaction<TrackedPayload> tx(health);
    tx.get().resize(300);
    throw std::runtime_error("Simulated Failure!");
  } catch (const std::exception& e) {
    std::cout << "Exception caught: " << e.what() << std::endl;
  }

  std::cout << "Final Size: " << health.size() << std::endl;
}

void demo_3_undoable_component() {
  UndoableComponent<TrackedPayload> hp(TrackedPayload("HP", 100));
  std::cout << "Initial Size: " << hp.value().size() << std::endl;
  hp.edit().resize(200);
  std::cout << "After edit().resize(200): " << hp.value().size() << std::endl;
  hp.undo();
  std::cout << "After undo(): " << hp.value().size() << std::endl;
  hp.commit();
  std::cout << "After commit(): " << hp.value().size() << std::endl;
  hp.edit().resize(50);
  std::cout << "After edit().resize(50): " << hp.value().size() << std::endl;
  hp.commit();
  std::cout << "After commit(): " << hp.value().size() << std::endl;
  std::cout << "Finale Size: " << hp.value().size() << std::endl;
}

int main() {
  demo_3_undoable_component();
}