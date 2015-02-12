#include <string>
#include <iostream>
#include <fstream>
 
int main()
{
    // read file line by line
	std::ifstream file("middle.txt");
	std::string line;
	while (std::getline(file, line)) {
		std::cout << line << "\n";
	}
}