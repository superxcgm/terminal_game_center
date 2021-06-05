#include "res.h"

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
    int len = strlen(buf);
    if (buf[len - 1] == '\n')
      buf[len - 1] = '\0'; /* remove newline character */
    data.push_back(buf);
  }
  fclose(fp);
}

void Resource::draw(int init_x, int init_y) {
  for (int i = 0; i < data.size(); ++i)
    mvaddstr(i + init_y, init_x, data[i].c_str());
}
