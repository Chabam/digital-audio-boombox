#include "DAB.hpp"

DAB::DAB(const char* path) {
    this->player = Player();

    this->show_files = std::filesystem::is_directory(path);
    if (!this->show_files) {
	    this->player.load_file(path);
    }
    this->init_windows();

    noecho();
    halfdelay(1);
    curs_set(0);
}

DAB::~DAB() {
	endwin();
}

void DAB::init_windows() {
    int max_x, max_y, lines, columns, offset_y;
    this->root = initscr();

    getmaxyx(this->root, max_y, max_x);

	lines = this->show_files ? max_y / 2 : max_y;
	columns = max_x;
    offset_y = this->show_files ? lines : 0;

	if (this->show_files) {
		this->file_explorer = this->init_window(columns, lines - 2, 0);
	}

    this->track_info = this->init_window(columns, lines - 2, offset_y);
}

WINDOW* DAB::init_window(
    const int width,
    const int height,
    const int offset_y
) {
    WINDOW* new_win = newwin(height, width, offset_y, 0);
    box(new_win, 0, 0);
    wrefresh(new_win);
    return new_win;
}

void DAB::display() {
    switch (getch()) {
		case 'p':
		case 'P': {
			this->player.play_pause();
			break;
		}
		case 'q':
		case 'Q': {
			return;
		}
	}
    this->display_track();
    if (this->show_files) {
        this->display_files();
    }
    this->display();
}

void DAB::display_track() {
    werase(this->track_info);
    wprintw(this->track_info, "Press 'p' to play/pause\n");
    wprintw(this->track_info, "Press 'q' to quit\n");
    wprintw(this->track_info, this->player.print_info().c_str());
    wrefresh(this->track_info);
}

void DAB::display_files() {
    werase(this->file_explorer);
    wprintw(this->file_explorer, "FILES\n");
    wrefresh(this->file_explorer);
}