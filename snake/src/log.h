#ifndef SNAKE_SRC_LOG_H_
#define SNAKE_SRC_LOG_H_

#define DEBUG_ON

#include <cstdio>

class Log {
 public:
  static void init();

  static void debug(const char* fmt, ...);

 private:
  static FILE* fp;
};

#endif  // SNAKE_SRC_LOG_H_
