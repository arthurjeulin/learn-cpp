// day1.cpp

#include <cmath> // for std::lround
#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <numeric>
#include <optional>
#include <random>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

struct Point2D {
  float x{};
  float y{};
};

struct TimedPoint {
  std::chrono::steady_clock::time_point ts{};
  Point2D p{};
};

struct Detection {
  int id{};
  float score{}; // 0..1
  Point2D center{};
};

struct LogLine {
  std::chrono::steady_clock::time_point ts{};
  std::string msg{};
};

static bool approx_equal(float a, float b, float eps = 1e-5f) {
  return (a > b ? a - b : b - a) <= eps;
}

[[nodiscard]] constexpr std::optional<Detection> first_detection_above(std::span<const Detection> dets,
                                               float threshold)
{
  if(dets.size() == 0)
    return std::nullopt;

  auto it = std::find_if(dets.begin(),dets.end(),[&](const Detection& det){
    return det.score >= threshold;
  });

  //return (it!= dets.end()) ? *it : std::nullopt;
  if(it!= dets.end())
    return *it;
  return std::nullopt;
}

// implement using std::ranges::transform
[[nodiscard]] std::vector<int> extract_ids(std::span<const Detection> dets)
{
  if(dets.size() == 0)
    return {};
  std::vector<int> out_ids;
  out_ids.resize(dets.size());
  std::transform(dets.begin(),dets.end(), out_ids.begin(),[](const Detection& det){
    return det.id;
  });
  
  return out_ids;
}

// implement using std::ranges::transform
/**
  * @brief Transformer les scores [0..1] en uint8_t [0..255] avec clamp
 */
[[nodiscard]] std::vector<std::uint8_t> scores_to_u8(std::span<const Detection> dets)
{
  if(dets.size() == 0)
    return {};
  std::vector<std::uint8_t> out_scores;
  out_scores.resize(dets.size());
  std::transform(dets.begin(),dets.end(),out_scores.begin(), [](const Detection& det){
    const int score = std::round(det.score * 255);
    return std::clamp(score, 0,255);
  });

  return out_scores;
}

static void day1_tests()
{
  std::vector<Detection> dets{
      {10, 0.30f, {0, 0}},
      {11, 0.95f, {1, 1}},
      {12, 0.70f, {2, 2}},
      {13, 0.95f, {3, 3}},
  };

  // Ex 1
  {
    auto d = first_detection_above(dets, 0.9f);
    assert(d.has_value());
    assert(d->id == 11); // first match, not best score
  }
  {
    auto d = first_detection_above(dets, 0.99f);
    assert(!d.has_value());
  }
  {
    std::vector<Detection> empty;
    auto d = first_detection_above(empty, 0.1f);
    assert(!d.has_value());
  }

  // Ex 2
  {
    auto ids = extract_ids(dets);
    assert((ids == std::vector<int>{10, 11, 12, 13}));
  }
  {
    std::vector<Detection> empty;
    auto ids = extract_ids(empty);
    assert(ids.empty());
  }

  // Bonus
  {
    std::vector<Detection> s{
        {1, -0.1f, {}},
        {2, 0.0f, {}},
        {3, 0.5f, {}},
        {4, 1.0f, {}},
        {5, 1.4f, {}},
    };
    auto u8 = scores_to_u8(s);
    assert(u8.size() == s.size());
    assert(u8[0] == 0);
    assert(u8[1] == 0);
    assert(u8[3] == 255);
    assert(u8[4] == 255);
    // 0.5 * 255 = 127.5 -> round -> 128
    assert(u8[2] == 128);
  }
}

int main()
{
  day1_tests();
  return 0;
}