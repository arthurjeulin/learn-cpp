#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <string>

int main(){
	std::vector<int> my_vec(100);
	std::iota(my_vec.begin(),my_vec.end(),0);
	std::string data_path = "data_text.txt";
	std::ofstream file(data_path);
	for(const auto& element: my_vec)
	{
		file << element << " ";
	}
	file << "\n";

	return 0;
}
