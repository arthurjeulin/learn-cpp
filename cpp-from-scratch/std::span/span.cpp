#include <iostream>
#include <span>
#include <vector>
#include <numeric>

/**
  @brief implement a simple function that print out a subset of our vector
  */
template <typename T>
constexpr void print_sub(const std::span<T> container)
{
  for(const auto& element: container)
  {
    std::cout << element << " ";
  }
  std::cout << std::endl;
}


int main()
{
  const std::size_t vector_size{20};
  std::vector<int> my_vec(vector_size);
  std::iota(my_vec.begin(),my_vec.end(),0);
  // conversion implicite de std::vector<int> en std::span<int>
  print_sub<int>(my_vec);
  // view a subset of my_vec
  const std::size_t input_offset{5};
  const std::size_t reading_element{10};
  print_sub<int>(std::span(my_vec.begin() + input_offset, reading_element));
  return 0;
}