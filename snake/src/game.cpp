#include "./game.h" /* NOLINT */

#include <ncurses.h>
#include <sys/time.h>

#include <ctime>

#include "./log.h"

void Game::init() {
  initscr();
  if (COLS < WIN_COLS || LINES < WIN_LINES) {
    endwin();
    fprintf(stderr, "Your console screen is smaller than %dx%d\n", WIN_COLS,
            WIN_LINES);
    fprintf(stderr, "Please resize your window and try again\n");
    exit(1);
  }
  Log::init();
  cbreak(); /* donot buffer input */
  noecho();
  curs_set(0);          /* donot display cursor */
  keypad(stdscr, TRUE); /* enable arrow key */
  /* ignore INT and QUIT signals */
  signal(SIGINT, SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
}

void Game::run() {
  while (true) {
    config = menu.draw_main();
    on_game();
  }
}

void Game::destroy() {
  curs_set(1); /* display cursor */
  endwin();
}

static Game *game_;

void redraw_snack(int signum) {
  if (!game_->queue_dir.empty()) {
    Log::debug("Extract direction from queue.\n");
    int new_direction = game_->queue_dir.front();
    Log::debug("Receive new direction: %d\n", new_direction);
    game_->queue_dir.pop();
    game_->the_snake.change_direction(new_direction);
  }
  game_->the_snake.update(game_->rect);

  if (game_->is_hit_wall()) {
    //        Log::debug("is real border: %d\n",
    //        game_->config.is_real_border());
    Log::debug("Hit the wall.\n");
    if (game_->config.is_real_wall()) {
      Log::debug("Real wall, game over.\n");
      game_->game_over();
    } else {
      game_->add_border_back();
      auto head = game_->the_snake.head();
      //            Log::debug("head(%d, %d), rect.right: %d, rect.bottom:
      //            %d\n", head.x, head.y, game_->rect.right(),
      //            game_->rect.bottom());
      Point new_point_on_other_side{0, 0};

      if (head.x >= game_->rect.right()) {
        new_point_on_other_side = {1, head.y};
      }
      if (head.x <= 0) {
        new_point_on_other_side = {game_->rect.right() - 1, head.y};
      }
      if (head.y >= game_->rect.bottom()) {
        new_point_on_other_side = {head.x, 1};
      }
      if (head.y <= 0) {
        new_point_on_other_side = {head.x, game_->rect.bottom() - 1};
      }
      game_->the_snake.add_head(new_point_on_other_side);
      game_->the_snake.add_head(new_point_on_other_side);
    }
  }

  if (game_->is_hit_fruit()) {
    // todo: duplicate tail add two node to snake which is not really good
    game_->the_snake.duplicate_tail();
    game_->draw_fruit();
  }

  if (game_->is_hit_body()) {
    Log::debug("Hit the body, game over!\n");
    game_->game_over();
  }
  game_->the_snake.print_snake();
  refresh();
}

void Game::add_border_back() {
  mvaddch(game_->the_snake.head().y, game_->the_snake.head().x, '.');
}

void Game::on_game() {
  int set_ticker(int n_msecs);
  int ch;
  int pre_ch;
  int delay;
  is_game_over = false;
  clear();
  rect.draw(config.is_real_wall());

  the_snake = Snake();
  the_snake.draw();

  draw_fruit();

  game_ = this;
  signal(SIGALRM, redraw_snack);
  delay = 20 * (10 - config.get_level());
  set_ticker(delay); /* speed of Snake would not change during play */
  pre_ch = ' ';
  while (true) {
    ch = getch();
    Log::debug("User press %c\n", ch);
    if (ch == pre_ch) /* ignore duplication key press */
      continue;
    pre_ch = ch;
    if (is_game_over) {
      switch (ch) {
        case 'm':
        case 'M':
          return; /* back to menu */
        case '\n':
          on_game(); /* just recursion */
          return;
        default:
          break;
      }
    }
    switch (ch) {
      case 'w':
      case 'W':
      case KEY_UP:
        /* use queue to buffer direction change request, if change
        the_snake.direction directly, later request will cover the
        previous request. */
        queue_dir.push(DIR_UP);
        break;
      case 's':
      case 'S':
      case KEY_DOWN:
        queue_dir.push(DIR_DOWN);
        break;
      case 'a':
      case 'A':
      case KEY_LEFT:
        queue_dir.push(DIR_LEFT);
        break;
      case 'd':
      case 'D':
      case KEY_RIGHT:
        queue_dir.push(DIR_RIGHT);
        break;
      case 'q':
      case 'Q':
        destroy();
        exit(0);
      default:
        break;
    }
  }
}

Game::Game()
    : rect(0, 0, WIN_COLS, WIN_LINES),
      menu(rect),
      res_game_over("res/game_over.res") {}

void Game::game_over() {
  int set_ticker(int n_msecs);
  int left_offset;

  attron(A_BOLD);
  mvaddch(game_->the_snake.head().y, game_->the_snake.head().x, 'x');
  attroff(A_BOLD);

  signal(SIGALRM, SIG_IGN);
  set_ticker(0); /* do not redraw Snake any more */

  left_offset =
      (rect.get_width() - strlen(res_game_over.get_line(0).c_str())) / 2;
  for (int i = 0; i < res_game_over.line_count(); ++i)
    mvaddstr(3 + i, left_offset, res_game_over.get_line(i).c_str());
  is_game_over = true;
  refresh();
}

void Game::draw_fruit() {
  do {
    std::uniform_int_distribution<int> dis_x(1, rect.right() - 1);
    std::uniform_int_distribution<int> dis_y(1, rect.bottom() - 1);
    fruit.x = rect.left() + dis_x(random_engine);
    fruit.y = rect.top() + dis_y(random_engine);
  } while (is_hit_snake(fruit));
  mvaddch(fruit.y, fruit.x, SYMBOL_FRUIT);
  refresh();
}

bool Game::is_hit_body() {
  return the_snake.is_hit(the_snake.head(), true, rect, config.is_real_wall());
}

bool Game::is_hit_wall() {
  auto head = the_snake.head();
  return head.x == rect.left() || head.y == rect.top() ||
         head.x == rect.right() || head.y == rect.bottom();
}

bool Game::is_hit_fruit() {
  return the_snake.head().x == fruit.x && the_snake.head().y == fruit.y;
}

bool Game::is_hit_snake(const Point &point) {
  return the_snake.is_hit(point, false, rect, config.is_real_wall());
}
