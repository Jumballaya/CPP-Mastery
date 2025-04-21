#include <iostream>
#include <vector>

#include "ConstexprView.hpp"
#include "FieldOffsetTable.hpp"
#include "FieldProjection.hpp"
#include "FieldRange.hpp"
#include "IndexedSpan.hpp"
#include "SliceView.hpp"
#include "SparseSlice.hpp"

void demo_1_sliceview() {
  std::vector<int> numbers = {10, 20, 30, 40, 50, 60};

  // Create a view over the full vector
  SliceView<int> view_full(numbers.data(), numbers.size());
  std::cout << "Full view: ";
  for (const int& n : view_full) {
    std::cout << n << " ";
  }
  std::cout << "\n";

  // Create a subview (offset = 2, length = 3) => 30 40 50
  auto sub = view_full.subview(2, 3);
  std::cout << "Subview [2, 3]: ";
  for (const int& n : sub) {
    std::cout << n << " ";
  }
  std::cout << "\n";

  // Out-of-bounds subview (should be empty)
  auto empty = view_full.subview(10, 5);
  std::cout << "Empty subview size: " << empty.size() << "\n";

  // Access operator[] test
  std::cout << "Index 1 in full view: " << view_full[1] << "\n";  // 20

  // Manual data pointer test
  const int* raw = view_full.data();
  std::cout << "First element via data(): " << *raw << "\n";  // 10
}

void demo_2_sliceview_for_loop() {
  std::vector<int> numbers = {1, 2, 3, 4, 5};
  SliceView<int> view(numbers.data(), numbers.size());

  std::cout << "Front: " << view.front() << "\n";
  std::cout << "Back: " << view.back() << "\n";

  for (auto x : view) {
    std::cout << x << " ";
  }
}

void demo_3_constexpr_view() {
  constexpr std::array<int, 5> base = {10, 20, 30, 40, 50};
  auto view = ConstexprView<int, 5>::from_array(base);

  static_assert(view.size() == 5, "Size check failed");

  std::cout << "ConstexprView contents: ";
  for (auto val : view) {
    std::cout << val << " ";
  }
  std::cout << "\n";

  std::cout << "First element: " << view.front() << "\n";
  std::cout << "Last element: " << view.back() << "\n";
}

struct CharacterStats {
  int health;
  float speed;
  char state;
};

void demo_4_field_offset_table() {
  using Stats = CharacterStats;
  FieldOffsetTable<Stats> table;

  table.registerField("health", &Stats::health);
  table.registerField("speed", &Stats::speed);
  table.registerField("state", &Stats::state);

  std::cout << "--- Field Offsets ---" << std::endl;
  for (const auto& key : table.keys()) {
    std::cout << key << " offset: " << table.offsetOf(key) << std::endl;
  }

  // Create sample object
  Stats stat = {100, 3.5f, 'A'};

  // Raw memory pointer
  const uint8_t* raw = reinterpret_cast<const uint8_t*>(&stat);

  std::cout << "--- Field Values via Offset ---" << std::endl;
  for (const auto& key : table.keys()) {
    size_t offset = table.offsetOf(key);

    if (key == "health") {
      const int* val = reinterpret_cast<const int*>(raw + offset);
      std::cout << key << ": " << *val << std::endl;
    } else if (key == "speed") {
      const float* val = reinterpret_cast<const float*>(raw + offset);
      std::cout << key << ": " << *val << std::endl;
    } else if (key == "state") {
      const char* val = reinterpret_cast<const char*>(raw + offset);
      std::cout << key << ": " << *val << std::endl;
    }
  }
}

struct StatBlock {
  int health;
  float speed;
  char state;
};

void demo_5_field_range() {
  StatBlock stats[] = {
      {100, 3.5f, 'A'},
      {80, 2.2f, 'B'},
      {50, 1.1f, 'C'}};

  FieldRange<StatBlock, float> speeds(stats, 3, &StatBlock::speed);

  std::cout << "--- Speeds Before ---" << std::endl;
  for (auto& s : speeds) {
    std::cout << s << std::endl;
    s += 1.0f;
  }

  std::cout << "--- Speeds After ---" << std::endl;
  for (auto& s : speeds) {
    std::cout << s << std::endl;
  }
}

struct Stat {
  float health;
  float stamina;
  float mana;
};

void demo_6_field_projection() {
  std::vector<Stat> stats = {
      {100.0f, 50.0f, 30.0f},
      {80.0f, 40.0f, 60.0f},
      {60.0f, 70.0f, 90.0f}};

  FieldProjection<Stat> projection(stats.data(), stats.size());

  auto healths = projection.field(&Stat::health);
  auto manas = projection.field(&Stat::mana);

  std::cout << "--- Initial Healths ---" << std::endl;
  for (float h : healths) std::cout << h << std::endl;

  std::cout << "--- Initial Manas ---" << std::endl;
  for (float m : manas) std::cout << m << std::endl;

  for (float& h : healths) h -= 10;
  for (float& m : manas) m += 5;

  std::cout << "--- Updated Healths ---" << std::endl;
  for (float h : healths) std::cout << h << std::endl;

  std::cout << "--- Updated Manas ---" << std::endl;
  for (float m : manas) std::cout << m << std::endl;
}

void demo_7_indexed_span() {
  std::vector<int> numbers = {10, 20, 30, 40, 50};
  size_t indices[] = {4, 0, 3};

  IndexedSpan<int> view(numbers.data(), indices, 3);

  std::cout << "Original order:" << std::endl;
  for (size_t i = 0; i < view.size(); i++) {
    std::cout << "view[" << i << "] = " << view[i] << std::endl;
  }

  view[1] = 99;

  std::cout << "\nModified vector:" << std::endl;
  for (int n : numbers) {
    std::cout << n << " ";
  }
  std::cout << std::endl;
}

void demo_8_sparse_slice() {
  std::cout << "--- Demo: SparseSlice ---" << std::endl;

  // Base data
  std::vector<int> numbers = {10, 20, 30, 40, 50, 60};

  // Create sparse view over selected elements
  std::vector<int*> selected;
  selected.push_back(&numbers[0]);
  selected.push_back(&numbers[2]);
  selected.push_back(&numbers[5]);

  SparseSlice<int> sparse(std::move(selected));

  std::cout << "SparseSlice contents: ";
  for (size_t i = 0; i < sparse.size(); ++i) {
    std::cout << sparse[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "Iterating with range-based for loop: ";
  for (auto& val : sparse) {
    std::cout << *val << " ";
  }
  std::cout << std::endl;

  std::cout << "Modifying first element in slice..." << std::endl;
  sparse[0] += 5;

  std::cout << "Original vector: ";
  for (int val : numbers) std::cout << val << " ";
  std::cout << std::endl;
}

int main() {
  demo_8_sparse_slice();
}
