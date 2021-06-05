
#include "log.h"

#include <cstdarg>

FILE* Log::fp = nullptr;

void Log::debug(const char* fmt, ...) {
  if (fp == nullptr) {
    return;
  }
  va_list args;
  va_start(args, fmt);
  vfprintf(fp, fmt, args);
  va_end(args);
  fflush(fp);
}

void Log::init() {
  if (fp != nullptr) {
    return;
  }
#ifdef DEBUG_ON
  fp = fopen("log.txt", "w");
#endif
}
