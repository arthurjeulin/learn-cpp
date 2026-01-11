#include <fstream>
#include <string>
/**
 * basic_ofstream: implemnts high-level file stream output operations
 * ofstream std::basic_ofstream<char>
 */
 
int main()
{
  // Create output file we want to write to.
  std::string file_name ="data.txt";
  std::ofstream output(file_name.c_str());
  for(std::size_t i = 0; i < 10; i++)
  {
    // operator << less and less signed
    output << i * i << ' ';
  }
  output << "\n";
  return 0;
}
