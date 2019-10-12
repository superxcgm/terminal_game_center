# bounce
base on Understanding Unix/Linux Programming.
## Preview
![bounce](../img/bounce.jpg)
## Diffs to book
* draw boundry
* remove use key to control speed
* add paddle
* add `a` and `d` key to move the paddle
* add "Game Over" hint
* auto spped up during play the game
* add score

## Usage
1. Install `curses`
2. Download code.
3. Compile, `gcc -o bounce2d bounce2d.c set_ticker.c -lcurses`.
4. Run it, `./bounce2d `.