#include "Structs.hpp"

namespace memo {

Rows::Rows(int value) : value(value)
{
}

Cols::Cols(int value) : value(value)
{
}

PosY::PosY(int value) : value(value)
{
}

PosX::PosX(int value) : value(value)
{
}

Size::Size() : height(0), width(0)
{
}

Size::Size(Height height, Width width) :
    height(height.value),
    width(width.value)
{
}

bool Size::operator==(const Size& other) const
{
    return width == other.width && height == other.height;
}

bool Size::operator!=(const Size& other) const
{
    return width != other.width && height != other.height;
}

Position::Position() : x(0), y(0)
{
}

Position::Position(PosX x, PosY y) :
    x(x.value),
    y(y.value)
{
}

bool Position::operator==(const Position& other) const
{
    return x == other.x && y == other.y;
}

bool Position::operator!=(const Position& other) const
{
    return x != other.x && y != other.y;
}

Rect::Rect(const Position& pos)
    : x(pos.x), y(pos.y)
{
}

Rect::Rect(const Size& size)
    : height(size.height), width(size.width)
{
}

Rect::Rect(const Position& pos, const Size& size)
    : x(pos.x), y(pos.y)
    , height(size.height), width(size.width)
{
}

void Rect::setPosition(const Position& pos)
{
    x = pos.x;
    y = pos.y;
}

Position Rect::position() const
{
    return { PosX(x), PosY(y) };
}

void Rect::setSize(const Size& size)
{
    width = size.width;
    height = size.height;
}

Size Rect::size() const
{
    return { Height(height), Width(width) };
}

bool Rect::operator==(const Rect& other) const
{
    return x == other.x 
        && y == other.y
        && height == other.height
        && width  == other.width;
}
bool Rect::operator!=(const Rect& other) const
{
    return !operator==(other);
}

} // namespace memo
