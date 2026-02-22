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