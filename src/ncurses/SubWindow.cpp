#include "SubWindow.hpp"
#include <ncurses.h>

#include <utility>

namespace memo {
namespace curses {


SubWindow::SubWindow(IWindow& parent) :
    SubWindow(parent, Position(), Size())
{
}

SubWindow::SubWindow(IWindow& parent, const Size& size) :
    SubWindow(parent, Position(), size)
{
}

SubWindow::SubWindow(IWindow& parent, const Position& position) :
    SubWindow(parent, position, Size())
{
}

SubWindow::SubWindow(IWindow& parent, const Position& position, const Size& size) :
    BaseWindow(position, size),
    parent_(&parent),
    window_(derwin(parent.cursesWindow(),
                   size.height, size.width,
                   position.y, position.x))
{
}

SubWindow::SubWindow(const SubWindow& other) :
    BaseWindow(other),
    parent_(other.parent_),
    window_(derwin(other.parent()->cursesWindow(),
            other.height(), other.width(),
            other.posY(), other.posX()))
{
}

SubWindow::SubWindow(SubWindow&& other) noexcept :
    BaseWindow(std::move(other)),
    parent_(other.parent_),
    window_(other.window_)
{
    other.window_ = nullptr;
}

SubWindow::~SubWindow()
{
    parent_ = nullptr;
    deleteCursesWindow(window_);
}

_win_st* SubWindow::cursesWindow() const
{
    return window_;
}

IWindow* SubWindow::parent() const
{
    return parent_;
}

SubWindow& SubWindow::operator=(const SubWindow& other)
{
    if (this == &other)
        return *this;

    BaseWindow::operator=(other);
    parent_ = other.parent_;
    return *this;
}

SubWindow& SubWindow::operator=(SubWindow&& other)
{
    if (this == &other)
        return *this;

    if (!deleteCursesWindow(window_))
        return *this;

    BaseWindow::operator=(std::move(other));
    parent_ = other.parent_;
    window_ = other.window_;
    other.window_ = nullptr;

    return *this;
}

int SubWindow::resizeWindow(_win_st* window, const memo::Size& newSize) const
{
    return wresize(window, newSize.height, newSize.width);
}

int SubWindow::moveWindow(_win_st* window, const memo::Position& newPosition) const
{
    return mvderwin(window, newPosition.y, newPosition.x);
}

} // namespace curses
} // namespace memo
