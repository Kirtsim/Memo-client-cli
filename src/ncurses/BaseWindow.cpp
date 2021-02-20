#include "ncurses/BaseWindow.hpp"
#include "ncurses/functions.hpp"

#include <ncurses.h>

namespace memo {
namespace curses {

BaseWindow::BaseWindow(const Position& position, const Size& size) :
    position_(position),
    size_(CorrectSize(size)),
    border_(DefaultBorder())
{
}

BaseWindow::BaseWindow(const BaseWindow& other) :
    position_(other.position_),
    size_(other.size_),
    border_(other.border_)
{
}

BaseWindow::BaseWindow(BaseWindow&& other) noexcept :
    BaseWindow(other)
{
}

BaseWindow& BaseWindow::operator=(const BaseWindow& other)
{
    if (this != &other)
    {
        setPosition(other.position_);
        setSize(other.size_);
        setWindowBorder(other.border_);
    }
    return *this;
}

BaseWindow& BaseWindow::operator=(BaseWindow&& other)
{
    if (this != &other)
    {
        position_ = other.position_;
        size_ = other.size_;
        border_ = other.border_;
    }
    return *this;
}

bool BaseWindow::redraw()
{
    return wrefresh(cursesWindow()) == OK;
}

bool BaseWindow::setPosition(const Position& newPosition)
{
    if (moveWindow(cursesWindow(), newPosition) == OK)
    {
        position_ = newPosition;
        return true;
    }
    return false;
}

bool BaseWindow::setX(const int newX)
{
    auto newPosition = position_;
    newPosition.x = newX;
    return setPosition(newPosition);
}

bool BaseWindow::setY(const int newY)
{
    auto newPosition = position_;
    newPosition.y = newY;
    return setPosition(newPosition);
}

const Position& BaseWindow::position() const
{
    return position_;
}

int BaseWindow::posX() const
{
    return position_.x;
}

int BaseWindow::posY() const
{
    return position_.y;
}

bool BaseWindow::setSize(const Size& size)
{
    const auto newSize = CorrectSize(size);

    if (resizeWindow(cursesWindow(), newSize) == OK)
    {
        size_ = newSize;
        return true;
    }
    return false;
}

bool BaseWindow::setHeight(const int newHeight)
{
    auto newSize = size_;
    newSize.height = newHeight;
    return setSize(newSize);
}

bool BaseWindow::setWidth(const int newWidth)
{
    auto newSize = size_;
    newSize.width = newWidth;
    return setSize(newSize);
}

const Size& BaseWindow::size() const
{
    return size_;
}

int BaseWindow::width() const
{
    return size_.width;
}

int BaseWindow::height() const
{
    return size_.height;
}

bool BaseWindow::setWindowBorder(const Border& newBorder)
{
    int result =
        wborder(cursesWindow(),
                border_.left, border_.right,
                border_.top, border_.bottom,
                border_.corner.upperLeft,
                border_.corner.upperRight,
                border_.corner.lowerLeft,
                border_.corner.lowerRight);
    if (result == OK)
    {
        border_ = newBorder;
        return true;
    }
    return false;
}

const Border& BaseWindow::windowBorder() const
{
    return border_;
}

void BaseWindow::erase() const
{
    wborder(cursesWindow(), ' ', ' ', ' ',' ',' ',' ',' ',' ');
}

bool BaseWindow::deleteCursesWindow(_win_st*& window)
{
    if (!window)
        return true;

    if (delwin(window) != OK && window)
        return false;

    window = nullptr;
    return true;
}

} // namespace curses
} // namespace memo
