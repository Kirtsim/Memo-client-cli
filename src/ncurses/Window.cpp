#include "Window.hpp"
#include "ncurses.h"

#include <utility>

namespace memo {
namespace curses {

Window::Window() :
    Window(Position(), Size())
{
}

Window::Window(const Size& size) :
    Window(Position(), size)
{
}

Window::Window(const Position& position) :
    Window(position, Size())
{
}

Window::Window(const Position& position, const Size& size) :
    BaseWindow(position, size),
    window_(newwin(size.height, size.width, position.y, position.x))
{
}

Window::Window(const Window& other) :
    BaseWindow(other),
    window_(newwin(other.height(), other.width(), other.posY(), posX()))
{
}

Window::Window(Window&& other) noexcept :
    BaseWindow(std::move(other)),
    window_(other.window_)
{
    other.window_ = nullptr;
}

Window::~Window()
{
    deleteCursesWindow(window_);
}

_win_st* Window::cursesWindow() const
{
    return window_;
}

Window& Window::operator=(const Window& other)
{
    if (this != &other)
        BaseWindow::operator=(other);

    return *this;
}

Window& Window::operator=(Window&& other)
{
    if (this == &other)
        return *this;

    if (!deleteCursesWindow(window_)) // make sure the old window is deleted
        return *this;

    BaseWindow::operator=(std::move(other));
    window_ = other.window_;
    other.window_ = nullptr;

    return *this;
}

int Window::resizeWindow(_win_st* window, const Size& newSize) const
{
    return wresize(window, newSize.height, newSize.width);
}

int Window::moveWindow(_win_st* window, const Position& newPosition) const
{
    return mvwin(window, newPosition.y, newPosition.x);
}

} // namespace curses
} // namespace memo
