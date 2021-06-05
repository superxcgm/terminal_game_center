#ifndef SNAKE_LOG_H
#define SNAKE_LOG_H

#define DEBUG_ON

#include <cstdio>

class Log {
public:
    static void init();

    static void debug(const char* fmt, ...);

private:
    static FILE *fp;
};


#endif //SNAKE_LOG_H
