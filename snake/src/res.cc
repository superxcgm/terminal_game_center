#include "./res.h" /* NOLINT */

#include <ncurses.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

Resource::Resource(const std::string &path) {
  FILE *fp = fopen(path.c_str(), "r");
  if (!fp) {
    perror("File opening failed");
    exit(1);
  }
  char buf[BUFSIZ];
  while (fgets(buf, BUFSIZ - 1, fp)) {
    int len = static_cast<int>(strlen(buf));
    if (buf[len - 1] == '\n')
      buf[len - 1] = '\0'; /* remove newline character */
    data_.emplace_back(buf);
  }
  fclose(fp);
}

void Resource::Draw(int init_x, int init_y) {
  for (int i = 0; i < data_.size(); ++i)
    mvaddstr(i + init_y, init_x, data_[i].c_str());
}
