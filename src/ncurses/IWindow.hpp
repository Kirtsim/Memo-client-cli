#pragma once

struct _win_st;

namespace memo {
    struct Position;
    struct Size;
    struct Border;
namespace curses {

///
/// \brief An interface for a ncurses' WINDOW wrapper.
///
class IWindow
{
public:
    virtual ~IWindow() = default;

    ///
    /// \brief Update the screen area occupied by this window with
    /// according to the current window state.
    /// \return Error code returned by ncurses.
    ///
    virtual bool redraw() = 0;

    virtual bool setPosition(const Position& newPosition) = 0;

    virtual bool setX(const int newX) = 0;

    virtual bool setY(const int newY) = 0;

    virtual const Position& position() const = 0;

    virtual int posX() const = 0;

    virtual int posY() const = 0;


    virtual bool setSize(const Size& newSize) = 0;

    virtual bool setHeight(const int newHeight) = 0;

    virtual bool setWidth(const int newWidth) = 0;

    virtual const Size& size() const = 0;

    virtual int width() const = 0;

    virtual int height() const = 0;


    virtual bool setWindowBorder(const Border& newBorder) = 0;

    virtual const Border& windowBorder() const = 0;

    virtual void erase() const = 0;

    ///
    /// \brief Get access to the underlying ncurses' WINDOW pointer.
    /// \return A pointer to the underlying ncurses' WINDOW.
    ///
    virtual _win_st* cursesWindow() const = 0;
};

} // namespace curses
} // namespace memo
