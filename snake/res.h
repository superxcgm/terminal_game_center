#ifndef SNAKE_RES_H
#define SNAKE_RES_H
#include <vector>
#include <string>

class Resource {
public:
    Resource(const std::string& path);
    int line_count() {
        return data.size();
    }
    std::string& get_line(int i) {
        return data[i];
    }
private:
    std::vector<std::string> data;
};
#endif