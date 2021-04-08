#include "keys.hpp"
#include <ncurses.h>

namespace memo {
namespace curses {

const int Key::kUp = KEY_UP;
const int Key::kDown = KEY_DOWN;
const int Key::kLeft = KEY_LEFT;
const int Key::kRight = KEY_RIGHT;

const int Key::kEnter = 10;
const int Key::kEsc = 27;
const int Key::kTab = 9;
const int Key::kShiftTab = KEY_BTAB;
const int Key::kBackSpace = 127;


} // namespace curses
} // namespace memo
