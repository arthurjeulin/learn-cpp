#include <fstream>
#include <iostream>
#include <string>
int main()
{
  std::string file_path = "data.txt";
  // read data from a file
  std::ifstream input(file_path);
  int data; // data we will read
  while(input >> data)
  {
    std::cout << data << " ";
  }
  std::cout << "\n";
  return 0;
}