### File I/O
standard library header `<fstream>`, this header is part of the **Input/Output** library.

**ofstream: output file stream.**
**ifstream: input file stream.**
#### Write Data`std::basic_ofstream`
The class template `basic_ofstream`implemnts high-level output operations on file based.  
It interfaces a filbased streamdbuffer (`std::basic_filebuf`) with the high-level interface of(`std::basic_ostream`)
```cpp
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
```
Run
```bash
g++ ofstream.cpp -std=c++20 -o test && ./test
```
Generate a `data.txt`
```txt
0 1 4 9 16 25 36 49 64 81 
```
### Read Data `std::basic_ifstream`
The class template `basic_ifstream` implements high-level input operation on file based streams.
It interfaces a file-basd streambuffer(`std::basic_filebuf`) with the high-level interface of (`std::basic_istream`)

```cpp
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
```