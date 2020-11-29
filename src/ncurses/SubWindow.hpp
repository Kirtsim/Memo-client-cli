#pragma once
#include "ncurses/BaseWindow.hpp"

namespace memo {
namespace curses {

///
/// \brief A wrapper class around the ncurses' WINDOW, representing a
/// derived window type, which has slightly different mechanics when it
/// comes to manipulating it's position and such.
///
class SubWindow : public BaseWindow
{
public:
    SubWindow(IWindow& parent);
    SubWindow(IWindow& parent, const Size& size);
    SubWindow(IWindow& parent, const Position& position);
    SubWindow(IWindow& parent, const Position& position, const Size& size);
    SubWindow(const SubWindow& other);
    SubWindow(SubWindow&& other) noexcept;

    virtual ~SubWindow();

    virtual _win_st* cursesWindow() const override;

    IWindow* parent() const;

    SubWindow& operator=(const SubWindow& other);
    SubWindow& operator=(SubWindow&& other);

protected:
    int resizeWindow(_win_st* window, const Size& newSize) const override;

    int moveWindow(_win_st* window, const Position& newPosition) const override;

private:
    IWindow* parent_;
    _win_st* window_;
};

} // namespace curses
} // namespace memo
