#pragma once

namespace memo {
namespace ui {

struct Rows
{
    explicit Rows(int iValue);

    int value;
};

using Height = Rows;

struct Cols
{
    explicit Cols(int iValue);

    int value;
};

using Width = Cols;

struct PosX
{
    explicit PosX(int iValue);

    int value;
};

struct PosY
{
    explicit PosY(int iValue);

    int value;
};

struct Size
{
    Size();
    Size(Height iHeight, Width iWidth);

    int height, width;
};

struct Position
{
    Position();
    Position(PosX iX, PosY iY);

    int x, y;
};

} // namespace ui
} // namespace memo
