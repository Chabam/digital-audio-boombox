#include "DAB.hpp"
#define GREEN_FOREGROUND 1
#define RED_FOREGROUND 2
#define YELLOW_FOREGROUND 3

DAB::DAB(const char* path) {
	this->player = Player();

	if (!std::experimental::filesystem::is_directory(path)) {
		std::cerr << "The path is not a valid directory path" << std::endl;
		std::cerr << "Usage:" << std::endl << "dab DIRECTORY" << std::endl;
		exit(1);
	}

	this->current_path = std::experimental::filesystem::absolute(path);

	this->load_directory();
	this->selected_index = 0;

	this->init_windows();
	noecho();
	halfdelay(1);
	curs_set(0);
	keypad(stdscr, TRUE);
	start_color();
	use_default_colors();
	assume_default_colors(-1,-1);
	init_pair(GREEN_FOREGROUND, COLOR_GREEN, -1);
	init_pair(RED_FOREGROUND, COLOR_RED, -1);
	init_pair(YELLOW_FOREGROUND, COLOR_YELLOW, -1);
}

DAB::~DAB() {
	delwin(this->file_explorer);
	delwin(this->track_info);
	delwin(this->root);
	endwin();
}

void DAB::init_windows() {
	this->root = initscr();

	if (this->file_explorer != nullptr || this->track_info != nullptr) {
		delwin(this->file_explorer);
		delwin(this->track_info);
	}

	this->file_explorer = this->init_window(COLS, LINES / 2, 0);
	this->track_info = this->init_window(COLS, (LINES / 2), (LINES / 2));
}

WINDOW* DAB::init_window(
	const int width,
	const int height,
	const int offset_y
) {
	WINDOW* new_win = newwin(height, width, offset_y, 0);
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
		case KEY_RIGHT: {
			this->player.fast_forward();
			break;
		}
		case KEY_LEFT: {
			this->player.rewind();
			break;
		}
		case KEY_UP: {
			this->selected_index = this->selected_index == 0
				? this->selected_index
				: this->selected_index - 1;
			break;
		}
		case KEY_DOWN: {
			this->selected_index = this->selected_index + 1 == this->current_files.size()
				? this->selected_index
				: this->selected_index + 1;
			break;
		}
		case '\n':
		case KEY_ENTER: {
			try {
				this->player.load_file(this->current_files[this->selected_index].c_str());
			} catch (...) {
				werase(this->file_explorer);
				wprintw(this->file_explorer, "Invalid music file!");
				wrefresh(this->file_explorer);
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
			break;
		}
		case 'q':
		case 'Q': {
			return;
		}
	}
	this->display_track();
	this->display_files();
	this->display();
}

void DAB::display_track() {
	werase(this->track_info);

	wprintw(this->track_info, this->player.print_info().c_str());

	wattron(this->track_info, A_BOLD | COLOR_PAIR(YELLOW_FOREGROUND));
	wprintw(this->track_info, "\nUsage :\n\t- 'p' to play/pause\n\t- Left and Right to fast-forward or rewind a track\n\t- 'q' to quit\n\t- Up and Down to move the cursor\n\t- Enter to select a file\n");
	wattroff(this->track_info, A_BOLD | COLOR_PAIR(YELLOW_FOREGROUND));

	wrefresh(this->track_info);
}

void DAB::display_files() {
	werase(this->file_explorer);
	wattron(this->file_explorer, A_BOLD | COLOR_PAIR(RED_FOREGROUND));
	wprintw(this->file_explorer, "%s\n", this->current_path.string().c_str());
	wattroff(this->file_explorer, A_BOLD | COLOR_PAIR(RED_FOREGROUND));

	waddch(this->file_explorer, ACS_VLINE);
	waddch(this->file_explorer, '\n');
	for (size_t i = this->selected_index; i < this->current_files.size(); ++i) {
		if (i == this->current_files.size() - 1) {
			 waddch(this->file_explorer, ACS_LLCORNER);
		} else {
			 waddch(this->file_explorer, ACS_LTEE);
		}

		if (i == this->selected_index) {
			wattron(this->file_explorer, A_BOLD | COLOR_PAIR(GREEN_FOREGROUND));
			wprintw(this->file_explorer, "%s\n", this->current_files[i].filename().string().c_str());
			wattroff(this->file_explorer, A_BOLD | COLOR_PAIR(GREEN_FOREGROUND));
		} else {
			wprintw(this->file_explorer, "%s\n", this->current_files[i].filename().string().c_str());
		}
	}
	mvwhline(this->file_explorer, (LINES / 2) - 1, 0, ACS_HLINE, COLS);
	wrefresh(this->file_explorer);
}

void DAB::load_directory() {
	this->current_files = std::vector<std::experimental::filesystem::path>();
	for (const auto & entry : std::experimental::filesystem::directory_iterator(this->current_path)) {
		this->current_files.push_back(entry.path());
	}
}