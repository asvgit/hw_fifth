#include <iostream>

int main() {
	try {
		std::cout << "hw_fifth" << std::endl;
	} catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
