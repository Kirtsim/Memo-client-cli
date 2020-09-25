#pragma once

namespace memo {
namespace ui {

struct Size
{
    int height, width;
};

struct Position
{
    int y, x;
};

class IComponent
{
public:
    virtual ~IComponent() = default;

    virtual void setHeight(int iHeight) = 0;
    virtual void setWidth(int iWidth) = 0;
    virtual void setSize(const Size& iSize) = 0;
    virtual int getHeight() const = 0;
    virtual int getWidth() const = 0;
    virtual Size getSize() const = 0;

    virtual void setY(int iY) = 0;
    virtual void setX(int iX) = 0;
    virtual void setPosition(const Position& iPos) = 0;
    virtual int getY() const = 0;
    virtual int getX() const = 0;
    virtual Position getPosition() const = 0;
};

} // namespace ui
} // namespace memo
