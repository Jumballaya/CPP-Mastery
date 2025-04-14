#include <algorithm>
#include <iostream>

#include "EntityContainer.hpp"
#include "OwningHandle.hpp"
#include "TrackedBuffer.hpp"
#include "TrackedComponent.hpp"
#include "TrackedObject.hpp"
#include "TrackedPayload.hpp"
#include "TrackedVector.hpp"

TrackedObject track_value(TrackedObject to) {
  std::cout << "[Function] - By Value" << std::endl;
  return to;
}

TrackedObject& track_value_ref(TrackedObject& to) {
  std::cout << "[Function] - By Ref" << std::endl;
  return to;
}

TrackedObject make_temp() {
  TrackedObject t("T");
  return t;
}

void demo_1_tracked_object() {
  TrackedObject to1("Test 1");
  TrackedObject to2("Test 2");
  TrackedObject to3 = make_temp();
  track_value(std::move(to1));
  track_value(to1);
  track_value(to3);
  track_value(to2);
  track_value_ref(to2);
  to1 = to2;
  to2 = to3;
  to3 = to1;

  std::vector<TrackedObject> vec;
  vec.push_back(TrackedObject("Temp"));
  vec.emplace_back("vec emplace back");
  TrackedObject a("A");
  vec.push_back(a);
  vec.push_back(std::move(a));
}

void demo_2_tracked_payload() {
  std::vector<TrackedPayload> vec;

  vec.emplace_back("First", 10);   // Direct in-place construction
  vec.emplace_back("Second", 20);  // Direct in-place construction

  TrackedPayload temp("Third", 30);  // Constructed
  vec.push_back(temp);               // Copy into vector
  vec.push_back(std::move(temp));    // Move into vector

  TrackedObject::print_stats();  // Print lifetime stats
}

void demo_3_owning_handle() {
  OwningHandle h1;
  OwningHandle h2 = h1;
  OwningHandle h3;
  h3 = h1;
}

void demo_4_owning_handle_vector() {
  static_assert(std::is_move_constructible<OwningHandle>::value, "OwningHandle is not move constructible");
  std::vector<OwningHandle> handles;
  handles.reserve(2);      // no realloc on the first couple of inserts
  handles.emplace_back();  // default ctor
  handles.emplace_back();

  std::cout << "--Resizing--" << std::endl;
  handles.push_back(OwningHandle());  // forces realloc
}

void demo_5_owning_handle_move_behavior() {
  OwningHandle h1;
  OwningHandle h2 = std::move(h1);  // move constructor
  OwningHandle h3;
  h3 = std::move(h2);  // move assignment
}

void demo_6_owning_handle_vector_moves() {
  std::vector<OwningHandle> handles;
  handles.reserve(1);  // force early reallocation

  handles.emplace_back();  // creates [0]
  handles.emplace_back();  // triggers move of [0], then adds [1]
}

void demo_7_tracked_buffer() {
  TrackedBuffer buffer("String Buffer", 10);
  std::string text = "Hello!";
  std::string text2 = " more text!";
  std::string text3 = " bit more";
  buffer.append(text.c_str(), text.size());
  buffer.append(text2.c_str(), text2.size());
  buffer.append(text3.c_str(), text3.size());
  std::string_view view(buffer.read<char>(0), buffer.size());
  std::cout << view << std::endl;
  std::cout << buffer << std::endl;
}

void demo_8_tracked_vector() {
  std::cout << "\n--- Creating v1 ---\n";
  TrackedVector<TrackedPayload> v1;
  v1.push_back(TrackedPayload("One", 5));
  v1.push_back(TrackedPayload("Two", 10));

  std::cout << "\n--- Copy constructing v2 from v1 ---\n";
  TrackedVector<TrackedPayload> v2 = v1;

  std::cout << "\n--- Move constructing v3 from v1 ---\n";
  TrackedVector<TrackedPayload> v3 = std::move(v1);

  std::cout << "\n--- Copy assigning v4 = v2 ---\n";
  TrackedVector<TrackedPayload> v4;
  v4 = v2;

  std::cout << "\n--- Move assigning v5 = std::move(v3) ---\n";
  TrackedVector<TrackedPayload> v5;
  v5 = std::move(v3);

  std::cout << "\n--- Clearing v5 ---\n";
  v5.clear();

  std::cout << "\n--- End of demo_3_tracked_vector ---\n";
}

void process_by_copy(TrackedPayload pl) {
  std::cout << "Process By Copy" << std::endl;
  TrackedVector<TrackedPayload> v1;
  v1.push_back(pl);
}

void process_by_move(TrackedPayload&& pl) {
  std::cout << "Process By Move" << std::endl;
  TrackedVector<TrackedPayload> v1;
  v1.push_back(std::move(pl));
  TrackedPayload cpy = std::move(pl);
  v1.push_back(cpy);
}

void process_by_ref(const TrackedPayload& pl) {
  std::cout << "Process By Ref" << std::endl;
  TrackedVector<TrackedPayload> v1;
  v1.push_back(pl);
}

void demo_9_tradeoffs() {
  TrackedPayload original("original", 1000);
  process_by_copy(original);
  process_by_move(std::move(original));
  process_by_ref(original);

  TrackedObject::print_stats();
}

void demo_10_tracked_component() {
  TrackedComponent tc1;
  TrackedComponent tc2("New Component", TrackedPayload("New Payload", 10));
  TrackedComponent tc3("New Component 2", "New Payload 2", 20);
  TrackedObject::print_stats();
}

void demo_11_entity_system() {
  EntityContainer<TrackedComponent> container;

  container.add(std::move(TrackedComponent("Health", "HP", 100)));
  container.add(std::move(TrackedComponent("Mana", "MP", 200)));
  container.add(std::move(TrackedComponent("Strength", "STR", 10)));
  container.add(std::move(TrackedComponent("Stamina", "STA", 10)));
  container.add(std::move(TrackedComponent("Dexterity", "DEX", 10)));
  container.add(std::move(TrackedComponent("Intelligence", "INT", 10)));
  container.add(std::move(TrackedComponent("Wisdom", "WIS", 10)));
  container.add(std::move(TrackedComponent("Charisma", "CHA", 10)));

  container.remove(0);

  TrackedObject::print_stats();
}

int main() {
  demo_11_entity_system();
}