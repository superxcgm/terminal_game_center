#ifndef SNAKE_MENU_H
#define SNAKE_MENU_H

#include "res.h"
#include "config.h"

class Menu {
public:
    Menu();

    Config draw_main();

private:
    void draw_control_menu(int flag, int base, const Config& config);

    Resource res_snake;
    Resource res_control_menu;
};


#endif //SNAKE_MENU_H
