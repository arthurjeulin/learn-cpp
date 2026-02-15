#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <functional>
/**
 * @brief Foncteur est un object callable dont operator() est défini.
 */
struct Multiply{
  int k {};
  int operator()(int x) const { return k * x;}
};

template <typename T, typename Pred>
std::vector<T> filter(const std::vector<T>& v, Pred pred)
{
  std::vector<T> out;
  out.reserve(v.size());
  std::copy_if(v.begin(),v.end(), std::back_inserter(out), pred);
  return out;
}

int main()
{
  {
    // First question
    Multiply mul{2};
    // Vector of size 10
    std::vector<int> my_vec(10);
    // Fill the vector with sequentially increasing values starting from 0
    std::iota(my_vec.begin(),my_vec.end(),0);
    for(const auto& element: my_vec)
    {
      std::cout << element << " ";
    }
    std::cout << std::endl;
    std::transform(my_vec.begin(),my_vec.end(),my_vec.begin(), mul);
    for(const auto& element: my_vec)
    {
      std::cout << element << " ";
    }
    std::cout << std::endl;    
  }
  {
    // Second question
    const int t = 3;
    // Vector of size 10
    std::vector<int> my_vec(10);
    // Fill the vector with sequentially increasing values starting from 0
    std::iota(my_vec.begin(),my_vec.end(),0);
    for(const auto& element: my_vec)
    {
      std::cout << element << " ";
    }
    std::cout << std::endl;
    auto my_lambda = [t](int x){ return t * x;};
    std::transform(my_vec.begin(),my_vec.end(),my_vec.begin(), my_lambda);
    for(const auto& element: my_vec)
    {
      std::cout << element << " ";
    }
    std::cout << std::endl;    
  }
  {
    // Third question
    const int t = 3;
    // Vector of size 10
    std::vector<int> my_vec(10);
    // Fill the vector with sequentially increasing values starting from 0
    std::iota(my_vec.begin(),my_vec.end(),0);
    for(const auto& element: my_vec)
    {
      std::cout << element << " ";
    }
    std::cout << std::endl;

    auto is_even = [](int x){ return (x % 2) == 0; };
    auto evens = filter(my_vec, is_even);

    for(const auto& element: evens)
    {
      std::cout << element << " ";
    }
    std::cout << std::endl;    
  }
  return 0;
}