#ifndef SNAKE_RES_H
#define SNAKE_RES_H
#include <vector>
#include <string>
#include <ncurses.h>

class Resource {
public:
    Resource(const std::string& path);
    int line_count() {
        return data.size();
    }
    std::string& get_line(int i) {
        return data[i];
    }

    void draw(int init_x, int init_y) {
        for (int i = 0; i < data.size(); ++i)
            mvaddstr(i + init_y, init_x, data[i].c_str());
    }
private:
    std::vector<std::string> data;
};
#endif