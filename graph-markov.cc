#include <iostream>
#include <fstream>
#include <iomanip>

int main()
{
	std::cout << "digraph Probabilities {\n";
	while (std::cin) {
		unsigned a, b;
		float prob;
		std::cin >> a >> b >> prob;

		std::cout << a << " -> " << b;
	}

	std::cout << "}\n";
}
