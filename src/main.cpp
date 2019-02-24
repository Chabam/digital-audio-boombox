#include "iostream"
#include "Player.hpp"

int main(int argc, char* argv[]) {
	std::cout << argc << std::endl;
	if (argc != 2) {
		std::cerr << "Usage:" << std::endl << "dab FILENAME" << std::endl;
		return 1;
	}

	char* fname = argv[1];
	Player player = Player();
	player.play_file(fname);
}