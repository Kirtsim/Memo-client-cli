#include "ncurses/functions.hpp"
#include "utils/Structs.hpp"

#include <ncurses.h>

namespace memo {
namespace curses {

class Window
{
public:
    WINDOW* getCursesWindow() const { return nullptr; }
};

void InitCurses()
{
    initscr();
}

void CloseCurses()
{
    endwin();
}

void Refresh()
{
    refresh();
}

void CursorVisible(bool visible)
{
    const int visibility = visible ? 1 : 0;
    curs_set(visibility);
}

void LineBuffering(bool enable)
{
    if (enable)
        nocbreak();
    else
        cbreak();
}

void PrintTypedInput(bool enable)
{
    if (enable)
        echo();
    else
        noecho();
}

void KeyPad(bool enable)
{
    int enabled = enable ? TRUE : FALSE;
    keypad(stdscr, enabled);
}

int ReadChar()
{
    return getch();
}

int ReadChar(const Window& window)
{
    return wgetch(window.getCursesWindow());
}

int ReadCharAt(const Position &position)
{
    return mvgetch(position.y, position.x);
}

int ReadCharAt(const Window &window, const Position &position)
{
    return mvwgetch(window.getCursesWindow(), position.y, position.x);
}





} // namespace curses
} // namespace memo
