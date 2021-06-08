#include "./game.h" /* NOLINT */

#include <ncurses.h>
#include <sys/time.h>

#include <ctime>

#include "./log.h"

void Game::Init() {
  initscr();
  if (COLS < WIN_COLS || LINES < WIN_LINES) {
    endwin();
    fprintf(stderr, "Your console screen is smaller than %dx%d\n", WIN_COLS,
            WIN_LINES);
    fprintf(stderr, "Please resize your window and try again\n");
    exit(1);
  }
    Log::Init();
  cbreak(); /* donot buffer input */
  noecho();
  curs_set(0);          /* donot display cursor */
  keypad(stdscr, TRUE); /* enable arrow key */
  /* ignore INT and QUIT signals */
  signal(SIGINT, SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
}

void Game::Run() {
  while (true) {
      config_ = menu_.DrawMain();
      OnGame();
  }
}

void Game::Destroy() {
  curs_set(1); /* display cursor */
  endwin();
}

static Game *game_;

void RedrawSnack(int signum) {
  if (!game_->queue_dir_.empty()) {
      Log::Debug("Extract direction from queue.\n");
    int new_direction = game_->queue_dir_.front();
      Log::Debug("Receive new direction: %d\n", new_direction);
    game_->queue_dir_.pop();
      game_->snake_.ChangeDirection(new_direction);
  }
    game_->snake_.Update(game_->rect_);

  if (game_->is_hit_wall()) {
    //        Log::Debug("is real border: %d\n",
    //        game_->config_.is_real_border());
      Log::Debug("Hit the wall.\n");
    if (game_->config_.is_real_wall()) {
        Log::Debug("Real wall, game over.\n");
        game_->GameOver();
    } else {
        game_->AddBorderBack();
      auto head = game_->snake_.head();
      //            Log::Debug("head(%d, %d), rect_.right: %d, rect_.bottom:
      //            %d\n", head.x_, head.y_, game_->rect_.right(),
      //            game_->rect_.bottom());
      Point new_point_on_other_side{0, 0};

      if (head.x >= game_->rect_.right()) {
        new_point_on_other_side = {1, head.y};
      }
      if (head.x <= 0) {
        new_point_on_other_side = {game_->rect_.right() - 1, head.y};
      }
      if (head.y >= game_->rect_.bottom()) {
        new_point_on_other_side = {head.x, 1};
      }
      if (head.y <= 0) {
        new_point_on_other_side = {head.x, game_->rect_.bottom() - 1};
      }
        game_->snake_.AddHead(new_point_on_other_side);
        game_->snake_.AddHead(new_point_on_other_side);
    }
  }

  if (game_->is_hit_fruit()) {
    // todo: duplicate tail add two node to snake which is not really good
      game_->snake_.DuplicateTail();
      game_->DrawFruit();
  }

  if (game_->is_hit_body()) {
      Log::Debug("Hit the body, game over!\n");
      game_->GameOver();
  }
  game_->snake_.print_snake();
  refresh();
}

void Game::AddBorderBack() {
  mvaddch(game_->snake_.head().y, game_->snake_.head().x, '.');
}

void Game::OnGame() {
  int set_ticker(int n_msecs);
  int ch;
  int pre_ch;
  int delay;
    is_game_over_ = false;
  clear();
    rect_.Draw(config_.is_real_wall());

    snake_ = Snake();
    snake_.Draw();

    DrawFruit();

  game_ = this;
    signal(SIGALRM, RedrawSnack);
  delay = 20 * (10 - config_.get_level());
  set_ticker(delay); /* speed of Snake would not change during play */
  pre_ch = ' ';
  while (true) {
    ch = getch();
      Log::Debug("User press %c\n", ch);
    if (ch == pre_ch) /* ignore duplication key press */
      continue;
    pre_ch = ch;
    if (is_game_over_) {
      switch (ch) {
        case 'm':
        case 'M':
          return; /* back to menu_ */
        case '\n':
            OnGame(); /* just recursion */
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
        snake_.direction directly, later request will cover the
        previous request. */
        queue_dir_.push(DIR_UP);
        break;
      case 's':
      case 'S':
      case KEY_DOWN:
        queue_dir_.push(DIR_DOWN);
        break;
      case 'a':
      case 'A':
      case KEY_LEFT:
        queue_dir_.push(DIR_LEFT);
        break;
      case 'd':
      case 'D':
      case KEY_RIGHT:
        queue_dir_.push(DIR_RIGHT);
        break;
      case 'q':
      case 'Q':
          Destroy();
        exit(0);
      default:
        break;
    }
  }
}

Game::Game()
    : rect_(0, 0, WIN_COLS, WIN_LINES),
      menu_(rect_),
      res_game_over_("res/game_over.res") {}

void Game::GameOver() {
  int set_ticker(int n_msecs);
  int left_offset;

  attron(A_BOLD);
  mvaddch(game_->snake_.head().y, game_->snake_.head().x, 'x');
  attroff(A_BOLD);

  signal(SIGALRM, SIG_IGN);
  set_ticker(0); /* do not redraw Snake any more */

  left_offset =
          (rect_.get_width() - strlen(res_game_over_.get_line(0).c_str())) / 2;
  for (int i = 0; i < res_game_over_.line_count(); ++i)
    mvaddstr(3 + i, left_offset, res_game_over_.get_line(i).c_str());
    is_game_over_ = true;
  refresh();
}

void Game::DrawFruit() {
  do {
    std::uniform_int_distribution<int> dis_x(1, rect_.right() - 1);
    std::uniform_int_distribution<int> dis_y(1, rect_.bottom() - 1);
      fruit_.x = rect_.left() + dis_x(random_engine_);
      fruit_.y = rect_.top() + dis_y(random_engine_);
  } while (is_hit_snake(fruit_));
  mvaddch(fruit_.y, fruit_.x, SYMBOL_FRUIT);
  refresh();
}

bool Game::is_hit_body() {
  return snake_.is_hit(snake_.head(), true, rect_, config_.is_real_wall());
}

bool Game::is_hit_wall() {
  auto head = snake_.head();
  return head.x == rect_.left() || head.y == rect_.top() ||
         head.x == rect_.right() || head.y == rect_.bottom();
}

bool Game::is_hit_fruit() {
  return snake_.head().x == fruit_.x && snake_.head().y == fruit_.y;
}

bool Game::is_hit_snake(const Point &point) {
  return snake_.is_hit(point, false, rect_, config_.is_real_wall());
}
