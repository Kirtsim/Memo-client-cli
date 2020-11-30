#pragma once
#include "ncurses/BaseWindow.hpp"

namespace memo {
namespace curses {

///
/// \brief A wrapper class around the ncurses' WINDOW
///
class Window : public BaseWindow
{
public:
    Window();
    Window(const Size& size);
    Window(const Position& position);
    Window(const Position& position, const Size& size);
    Window(const Window& other);
    Window(Window&& other) noexcept;
    virtual ~Window();

    virtual _win_st* cursesWindow() const override;

    Window& operator=(const Window& other);
    Window& operator=(Window&& other);

protected:
    int resizeWindow(_win_st* window, const Size& newSize) const override;

    int moveWindow(_win_st* window, const Position& newPosition) const override;

private:
    _win_st* window_;
};

} // namespace curses
} // namespace memo
