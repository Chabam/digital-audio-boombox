#include "ncurses.h"
#include "Player.hpp"
#include "filesystem"
#include "thread"

class DAB {
    WINDOW* root;
    WINDOW* track_info;
    WINDOW* file_explorer;
    Player player;
    bool show_files;
    void init_windows();
    WINDOW* init_window(const int, const int, const int);
    void display_files();
    void display_track();
public:
    DAB(const char*);
    ~DAB();
    void display();
};