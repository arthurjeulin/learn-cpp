#include <algorithm>
#include <random>

int main()
{
  // Allocate some memory
  const int N = 1 << 10;
  float *p = new float[N];
  // Set the contents in a for loop
  for (size_t i = 0;i < N ; i++)
  {
    p[i] = 10;
  }
  // Set the contents in a for loop
  for (size_t i = 0;i < N ; i++)
  {
    p[i] = 20;
  }
  
  // Free our memory
  delete[] p;

  return 0;
}
