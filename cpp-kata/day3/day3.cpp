// day3.cpp

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <ranges>
#include <span>
#include <vector>

struct Point2D { 
  float x{};
  float y{};
};

struct Detection {
  int id{};
  float score{};
  Point2D center{};
};

// TODO 1: std::ranges::sort + projection &Detection::id
void sort_by_id(std::vector<Detection>& dets)
{
  // Sort by id ascending (default comparator is std::ranges::less)
  std::ranges::sort(dets,{},&Detection::id);
}

// TODO 2: assumes sorted by id; use std::ranges::unique + erase
void unique_by_id_sorted(std::vector<Detection>& dets)
{
  // return a subranges pointing to the new end.
  auto [new_end, last] = std::ranges::unique(dets,{},&Detection::id);
  dets.erase(new_end,last);
}

// BONUS: std::ranges::equal_range + span view
[[nodiscard]] std::span<const Detection>
equal_range_id(std::span<const Detection> dets_sorted_by_id, int id)
{
  // 1. Range: dets_sorted_by_id
  // 2. Value: id (value_to_find)
  // 3. Comparator: {} (default std::ranges::less)
  // 4. Projection: &Detection::id (the fix)
  auto range = std::ranges::equal_range(dets_sorted_by_id, id, {}, &Detection::id);
  
  // Convert the resulting subrange back to a span
  return std::span<const Detection>(range);
}

static bool is_sorted_by_id(const std::vector<Detection>& dets)
{
  return std::ranges::is_sorted(dets, {}, &Detection::id);
}

static void day3_tests()
{
  std::vector<Detection> dets{
      {3, 0.2f, {}},
      {1, 0.9f, {}},
      {2, 0.1f, {}},
      {2, 0.8f, {}},
      {3, 0.5f, {}},
      {1, 0.7f, {}},
  };

  // Ex 1: sort by id
  sort_by_id(dets);
  assert(is_sorted_by_id(dets));

  // Ex 2: unique by id (after sort)
  unique_by_id_sorted(dets);
  assert(is_sorted_by_id(dets));
  // Expect ids {1,2,3}
  std::vector<int> ids;
  ids.reserve(dets.size());
  for (const auto& d : dets) ids.push_back(d.id);
  assert((ids == std::vector<int>{1, 2, 3}));

  // Bonus: equal_range
  {
    std::vector<Detection> v{
        {1, 0.1f, {}},
        {1, 0.2f, {}},
        {2, 0.3f, {}},
        {2, 0.4f, {}},
        {2, 0.5f, {}},
        {4, 0.6f, {}},
    };
    assert(is_sorted_by_id(v));

    auto r2 = equal_range_id(v, 2);
    assert(r2.size() == 3);
    assert(r2[0].id == 2 && r2[1].id == 2 && r2[2].id == 2);

    auto r3 = equal_range_id(v, 3);
    assert(r3.empty());

    auto r1 = equal_range_id(v, 1);
    assert(r1.size() == 2);
  }
}

int main()
{
  day3_tests();
  return 0;
}