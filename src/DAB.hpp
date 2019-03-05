#include "ncurses.h"
#include "Player.hpp"
#include "filesystem"
#include "thread"
#include "vector"
#include "sstream"
#include <chrono>
#include <thread>
#include <signal.h>

class DAB {
    WINDOW* root;
    WINDOW* track_info;
    WINDOW* file_explorer;
    Player player;
    std::filesystem::path current_path;
    size_t selected_index;
    std::vector<std::filesystem::path> current_files;
    void init_windows();
    WINDOW* init_window(const int, const int, const int);
    void display_files();
    void display_track();
    void load_directory();
public:
    DAB(const char*);
    ~DAB();
    void display();
};