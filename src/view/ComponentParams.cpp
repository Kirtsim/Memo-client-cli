#include "view/ComponentParams.hpp"

namespace memo {
namespace ui {

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

Position::Position() : x(0), y(0)
{
}

Position::Position(PosX iX, PosY iY) :
    x(iX.value),
    y(iY.value)
{
}

} // namespace ui
} // namespace memo
