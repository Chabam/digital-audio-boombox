#include "iostream"
#include "DAB.hpp"

int main(int argc, char* argv[]) {
	std::cout << argc << std::endl;
	if (argc != 2) {
		std::cerr << "Usage:" << std::endl << "dab (DIRECTORY|FILENAME)" << std::endl;
		return 1;
	}

	char* path = argv[1];
	DAB dab = DAB(path);
	dab.display();
}