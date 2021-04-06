#include "ncurses/functions.hpp"
#include "ncurses/IWindow.hpp"
#include "utils/Structs.hpp"

#include <ncurses.h>

namespace memo {
namespace curses {

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

Position CursorPosition(const IWindow& window)
{
    Position pos;
    getyx(window.cursesWindow(), pos.y, pos.x);
    return pos;
}

void PositionCursor(const Position& pos)
{
    move(pos.y, pos.x);
}

void PositionCursor(const IWindow& window, const Position& pos)
{
    wmove(window.cursesWindow(), pos.y, pos.x);
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

void KeyPad(const IWindow& window, bool enable)
{
    int enabled = enable ? TRUE : FALSE;
    keypad(window.cursesWindow(), enabled);
}

int ReadChar()
{
    return getch();
}

int ReadChar(const IWindow& window)
{
    return wgetch(window.cursesWindow());
}

int ReadCharAt(const Position& position)
{
    return mvgetch(position.y, position.x);
}

int ReadCharAt(const IWindow& window, const Position& position)
{
    return mvwgetch(window.cursesWindow(), position.y, position.x);
}

void PrintCharAt(int character, const Position& position)
{
    mvaddch(position.y, position.x, character);
}

void PrintCharAt(int character, const IWindow& window, const Position& position)
{
    mvwaddch(window.cursesWindow(), position.y, position.x, character);
}

int ScreenWidth()
{
    return COLS;
}

int ScreenHeight()
{
    return LINES;
}

Size ScreenSize()
{
    return { Height(LINES), Width(COLS) };
}

Size CorrectSize(const Size& size)
{
    auto result = size;
    if (size.height <= 0)
        result.height = ScreenHeight();
    if (size.width <= 0)
        result.width = ScreenWidth();
    return result;
}

int PrintText(const std::string& text, const Position& position)
{
    return mvprintw(position.y, position.x, text.c_str());
}

int PrintText(const std::string& text, const IWindow& window, const Position& position)
{
    return mvwprintw(window.cursesWindow(), position.y, position.x, text.c_str());
}

Border DefaultBorder()
{
    return {
        ACS_HLINE, ACS_VLINE, ACS_HLINE, ACS_VLINE,
        { ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LLCORNER }
    };
}

} // namespace curses
} // namespace memo
