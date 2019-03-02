#include "iostream"
#include "Player.hpp"
#include "ncurses.h"


void display_loop(Player* player);

int main(int argc, char* argv[]) {
	std::cout << argc << std::endl;
	if (argc != 2) {
		std::cerr << "Usage:" << std::endl << "dab FILENAME" << std::endl;
		return 1;
	}

	initscr();
	char* fname = argv[1];
	Player player = Player();
	player.load_file(fname);
	display_loop(&player);
	endwin();
}

void display_loop(Player* player) {
	printw("Press 'p' to play/pause\n");
	printw("Press 'q' to quit\n");
	switch (getch()) {
		case 'p':
		case 'P': {
			player->play_pause();
			break;
		}
		case 'q':
		case 'Q': {
			return;
		}
		default: {
			printw("Key not recongnized\n");
			break;
		}
	}
	refresh();
	clear();
	display_loop(player);
}