#include <iostream>
#include <fstream>
#include <string>

int main()
{
	const std::string data_path = "data_text.txt";
	// Open a file
	std::ifstream file(data_path);
	int data;
	while(file >> data)
	{
		std::cout << data << " ";
	}
	std::cout << std::endl;
	return 0;
}
