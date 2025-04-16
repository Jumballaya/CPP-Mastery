#include <iostream>

#include "../week01_resource_manager/MoveOnlyBuffer.hpp"
#include "../week02_tracked_object/TrackedPayload.hpp"
#include "RollbackStack.hpp"
#include "ScopedTransaction.hpp"
#include "UndoableComponent.hpp"
#include "UndoableRegistry.hpp"

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

void demo_4_rollback_stack() {
  std::vector<std::string> words = {
      "Hello",
      "World",
      "Apple",
      "Brown",
      "Limes",
      "Grape",
      "Short",
      "Brain",
      "Crabs",
      "Mount"};

  RollbackStack<MoveOnlyBuffer> stack;
  for (auto word : words) {
    MoveOnlyBuffer hello(5);
    hello.append(word.c_str(), 5);
    stack.push(std::move(hello));
  }
  std::cout << "Stack Top: " << stack.current().read<char>(0) << std::endl;
  while (stack.pop()) {
    std::cout << "Stack Top: " << stack.current().read<char>(0) << std::endl;
  }
}

void demo_5_rollback_stack_undoable_component() {
  RollbackStack<UndoableComponent<int>> stack;
  stack.push(UndoableComponent<int>(0));
  std::cout << "Stack Top: " << stack.current().value() << std::endl;
  stack.push(UndoableComponent<int>(1));
  std::cout << "Stack Top: " << stack.current().value() << std::endl;
  stack.push(UndoableComponent<int>(2));
  std::cout << "Stack Top: " << stack.current().value() << std::endl;
  stack.pop();
  std::cout << "Stack Top (p): " << stack.current().value() << std::endl;
  stack.pop();
  std::cout << "Stack Top (p): " << stack.current().value() << std::endl;
  stack.push(UndoableComponent<int>(3));
  std::cout << "Stack Top: " << stack.current().value() << std::endl;
  stack.push(UndoableComponent<int>(4));
  std::cout << "Stack Top: " << stack.current().value() << std::endl;
  stack.push(UndoableComponent<int>(5));
  std::cout << "Stack Top: " << stack.current().value() << std::endl;
}

void demo_6_undoable_registry() {
  UndoableRegistry<TrackedPayload> registry;

  registry.edit("Health").resize(100);
  std::cout << "Resize Health to 100, committed: " << registry.get("Health")->size() << std::endl;
  registry.commit("Health");
  std::cout << "Commit Health to: " << registry.get("Health")->size() << std::endl;

  registry.edit("Mana").resize(50);
  std::cout << "Resize Mana to 50, committed: " << registry.get("Mana")->size() << std::endl;
  registry.undo("Mana");
  std::cout << "Undo Mana to: " << registry.get("Mana")->size() << std::endl;
  registry.edit("Mana").resize(200);
  std::cout << "Resize Mana to 200, committed: " << registry.get("Mana")->size() << std::endl;
  registry.commit("Mana");
  std::cout << "Commit Mana to: " << registry.get("Mana")->size() << std::endl;

  registry.edit("Strength").resize(10);
  std::cout << "Resize Strength to 10, committed: " << registry.get("Strength")->size() << std::endl;
  registry.commit("Strength");
  std::cout << "Commit Strength to: " << registry.get("Strength")->size() << std::endl;
  registry.edit("Strength").resize(50);
  std::cout << "Resize Strength to 50, committed: " << registry.get("Strength")->size() << std::endl;
  registry.commit("Strength");
  std::cout << "Commit Strength to: " << registry.get("Strength")->size() << std::endl;

  TrackedObject::print_stats();
}

int main() {
  demo_6_undoable_registry();
}