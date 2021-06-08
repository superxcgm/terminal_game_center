
#include "./log.h" /* NOLINT */

#include <cstdarg>

FILE* Log::fp_ = nullptr;

void Log::Debug(const char* fmt, ...) {
  if (fp_ == nullptr) {
    return;
  }
  va_list args;
  va_start(args, fmt);
  vfprintf(fp_, fmt, args);
  va_end(args);
  fflush(fp_);
}

void Log::Init() {
  if (fp_ != nullptr) {
    return;
  }
#ifdef DEBUG_ON
    fp_ = fopen("log.txt", "w");
#endif
}
