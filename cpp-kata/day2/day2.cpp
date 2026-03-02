// day2.cpp

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <ranges>
#include <span>
#include <vector>
#include <numeric>

struct Point2D {
  float x{};
  float y{};
};

struct TimedPoint {
  std::chrono::steady_clock::time_point ts{};
  Point2D p{};
};

// TODO 1: use std::ranges::copy_if
[[nodiscard]] std::vector<TimedPoint>
filter_time_window(std::span<const TimedPoint> pts,
                   std::chrono::steady_clock::time_point t0,
                   std::chrono::steady_clock::time_point t1)
{
  std::vector<TimedPoint> out_pts;
  out_pts.reserve(pts.size());
  // check if pt.ts is in filter time_window
  std::ranges::copy_if(pts,std::back_inserter(out_pts),[&t0,&t1](const TimedPoint& pt)
  {
    return (pt.ts >= t0) && (pt.ts <= t1);
  });
  
  return out_pts;
}

// TODO 2: use std::ranges::count_if (no allocations)
[[nodiscard]] std::size_t
count_points_within_radius(std::span<const TimedPoint> pts,
                           std::chrono::steady_clock::time_point t0,
                           std::chrono::steady_clock::time_point t1,
                           float radius_m)
{
  // Check radius_m is positive value
  if(radius_m <0)
    return 0;
  radius_m = std::max(radius_m,0.0f);
  const float r2 = radius_m * radius_m;
  // count all point that verifie t0 <= ts <= t1
  // x*x + y*y <= raduis_m * radius_m
  auto count_close_points_inside_windws{std::ranges::count_if(pts,[&](const TimedPoint& pt)
  {
    const float d2 = pt.p.x*pt.p.x + pt.p.y*pt.p.y; 
    return  ((pt.ts >= t0) && (pt.ts <= t1) && (d2 <= r2 ));
  })};

  return static_cast<std::size_t>(count_close_points_inside_windws);
}

// // BONUS (optional): downsample
// [[nodiscard]] std::vector<TimedPoint>
// take_every_n(std::span<const TimedPoint> pts, std::size_t n)
// {
//   if (n == 0)
//     return {};

//   auto strided = pts | std::views::stride(n);
//   std::vector<TimedPoint> strided_output(strided.begin(), strided.end());
//   return strided_output;
// }

static void day2_tests()
{
  using namespace std::chrono;

  const auto base = steady_clock::now();
  const auto t0 = base + 10ms;
  const auto t1 = base + 30ms;

  std::vector<TimedPoint> pts{
      {base + 0ms,  {0.0f, 0.0f}},  // out
      {base + 10ms, {1.0f, 0.0f}},  // in
      {base + 15ms, {0.5f, 0.5f}},  // in
      {base + 30ms, {2.0f, 0.0f}},  // in (boundary)
      {base + 31ms, {0.0f, 0.0f}},  // out
  };

  // Ex 1: filter window
  {
    auto w = filter_time_window(pts, t0, t1);
    assert(w.size() == 3);
    assert(w[0].ts == base + 10ms);
    assert(w[1].ts == base + 15ms);
    assert(w[2].ts == base + 30ms);
  }

  // Ex 1: invalid window => empty
  {
    auto w = filter_time_window(pts, t1, t0);
    assert(w.empty());
  }

  // Ex 2: count within radius inside window
  // radius = 1.0 => r^2 = 1.0
  // points in window:
  // (1,0) => 1.0 (in)
  // (0.5,0.5) => 0.5 (in)
  // (2,0) => 4.0 (out)
  {
    const std::size_t n = count_points_within_radius(pts, t0, t1, 1.0f);
    assert(n == 2);
  }

  // Ex 2: negative radius treated as 0 => only points exactly at origin (none in window)
  {
    const std::size_t n = count_points_within_radius(pts, t0, t1, -1.0f);
    assert(n == 0);
  }

  // Bonus
  // {
  //   std::vector<TimedPoint> small{
  //       {base + 0ms,  {0,0}},
  //       {base + 1ms,  {1,0}},
  //       {base + 2ms,  {2,0}},
  //       {base + 3ms,  {3,0}},
  //       {base + 4ms,  {4,0}},
  //   };
  //   auto d = take_every_n(small, 2);
  //   assert(d.size() == 3);
  //   assert(d[0].p.x == 0);
  //   assert(d[1].p.x == 2);
  //   assert(d[2].p.x == 4);
  // }
  // {
  //   std::vector<TimedPoint> small{
  //       {base + 0ms, {0,0}},
  //       {base + 1ms, {1,0}},
  //   };
  //   auto d0 = take_every_n(small, 0);
  //   assert(d0.empty());
  // }
}

int main()
{
  day2_tests();
  return 0;
}