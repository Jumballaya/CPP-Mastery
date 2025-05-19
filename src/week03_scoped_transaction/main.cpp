#include <iostream>

#include "../week01_resource_manager/MoveOnlyBuffer.hpp"
#include "../week02_tracked_object/TrackedPayload.hpp"
#include "RollbackStack.hpp"
#include "ScopedTransaction.hpp"
#include "TransactionGroup.hpp"
#include "UndoableComponent.hpp"
#include "UndoableEntity.hpp"
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

void demo_7_undoable_entity() {
  UndoableEntity<TrackedPayload> stats;

  // Initial health
  stats.edit("Health").resize(100);
  stats.commit("Health");

  // Modify health but don't commit yet
  stats.edit("Health").resize(50);
  std::cout << "Health (dirty): " << stats.is_dirty("Health") << "\n";
  std::cout << "Health size (uncommitted): " << stats.edit("Health").size() << "\n";
  std::cout << "Health size (committed): " << stats.value("Health").size() << "\n";

  // Undo the edit
  stats.undo("Health");
  std::cout << "After undo: Health size = " << stats.value("Health").size() << "\n";

  // New stat: Strength
  stats.edit("Strength").resize(25);
  stats.commit("Strength");
  std::cout << "Strength size: " << stats.value("Strength").size() << "\n";

  // Modify both
  stats.edit("Health").resize(75);
  stats.edit("Strength").resize(10);
  stats.commitAll();
  std::cout << "After commitAll():\n";
  std::cout << "  Health = " << stats.value("Health").size() << "\n";
  std::cout << "  Strength = " << stats.value("Strength").size() << "\n";

  // Undo both
  stats.undoAll();
  std::cout << "After undoAll():\n";
  std::cout << "  Health = " << stats.value("Health").size() << "\n";
  std::cout << "  Strength = " << stats.value("Strength").size() << "\n";

  // Print out keys
  std::cout << "Keys in registry: ";
  for (const auto& k : stats.keys()) {
    std::cout << k << " ";
  }
  std::cout << "\n";

  TrackedObject::print_stats();
}

void demo_8_transaction_group() {
  std::cout << "\n--- Demo 8: TransactionGroup ---" << std::endl;

  TrackedPayload hp("HP", 100);
  TrackedPayload mp("MP", 50);
  TrackedPayload stamina("STA", 25);

  std::cout << "Before Group: HP=" << hp.size()
            << ", MP=" << mp.size()
            << ", STA=" << stamina.size() << std::endl;

  {
    TransactionGroup<TrackedPayload> group;
    group.add(hp);
    group.add(mp);
    group.add(stamina);

    group.get(0).resize(125);  // HP
    group.get(1).resize(75);   // MP
    group.get(2).resize(40);   // STA

    std::cout << "Within Group (Uncommitted): "
              << "HP=" << group.get(0).size()
              << ", MP=" << group.get(1).size()
              << ", STA=" << group.get(2).size() << std::endl;

    group.commitAll();
  }

  std::cout << "After Commit: HP=" << hp.size()
            << ", MP=" << mp.size()
            << ", STA=" << stamina.size() << std::endl;

  {
    TransactionGroup<TrackedPayload> rollbackGroup;
    rollbackGroup.add(hp);
    rollbackGroup.add(mp);

    rollbackGroup.get(0).resize(999);
    rollbackGroup.get(1).resize(999);

    std::cout << "Before Rollback: HP=" << rollbackGroup.get(0).size()
              << ", MP=" << rollbackGroup.get(1).size() << std::endl;
  }

  std::cout << "After Rollback: HP=" << hp.size()
            << ", MP=" << mp.size() << std::endl;

  TrackedObject::print_stats();
}

int main() {
  demo_8_transaction_group();
}