#include "Structs.hpp"

namespace memo {

Rows::Rows(int iValue) : value(iValue)
{
}

Cols::Cols(int iValue) : value(iValue)
{
}

PosY::PosY(int iValue) : value(iValue)
{
}

PosX::PosX(int iValue) : value(iValue)
{
}

Size::Size() : height(0), width(0)
{
}

Size::Size(Height iHeight, Width iWidth) :
    height(iHeight.value),
    width(iWidth.value)
{
}

bool Size::operator==(const Size& iOther) const
{
    return width == iOther.width && height == iOther.height;
}

bool Size::operator!=(const Size& iOther) const
{
    return width != iOther.width && height != iOther.height;
}

Position::Position() : x(0), y(0)
{
}

Position::Position(PosX iX, PosY iY) :
    x(iX.value),
    y(iY.value)
{
}

bool Position::operator==(const Position& iOther) const
{
    return x == iOther.x && y == iOther.y;
}

bool Position::operator!=(const Position& iOther) const
{
    return x != iOther.x && y != iOther.y;
}

} // namespace memo
