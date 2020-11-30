#pragma once
#include "ncurses/IWindow.hpp"
#include "utils/Structs.hpp"

namespace memo {
namespace curses {


///
/// \brief A base for all ncurses WINDOW wrappers. Encapsulates common
/// logic and properties.
///
class BaseWindow : public IWindow
{
public:
    BaseWindow(const Position& position, const Size& size);

    BaseWindow(const BaseWindow& other);

    BaseWindow(BaseWindow&& other) noexcept;

    BaseWindow& operator=(const BaseWindow& other);

    BaseWindow& operator=(BaseWindow&& other);

    virtual ~BaseWindow() = default;

    virtual bool setPosition(const Position& newPosition) override;

    virtual bool setX(const int newX) override;

    virtual bool setY(const int newY) override;

    virtual const Position& position() const override;

    virtual int posX() const override;

    virtual int posY() const override;


    virtual bool setSize(const Size& newSize) override;

    virtual bool setHeight(const int newHeight) override;

    virtual bool setWidth(const int newWidth) override;

    virtual const Size& size() const override;

    virtual int width() const override;

    virtual int height() const override;


    virtual bool setWindowBorder(const Border& newBorder) override;

    virtual const Border& windowBorder() const override;

protected:
    bool deleteCursesWindow(_win_st*& window);

    virtual int resizeWindow(_win_st* window, const Size& newSize) const = 0;

    virtual int moveWindow(_win_st* window, const Position& newPosition) const = 0;

private:
    Position position_;
    Size size_;
    Border border_;
};

} // namespace curses
} // namespace memo
