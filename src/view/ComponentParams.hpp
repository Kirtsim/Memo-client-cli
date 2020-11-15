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

    bool operator==(const Size& iOther);
    bool operator!=(const Size& iOther);

    int height, width;
};

struct Position
{
    Position();
    Position(PosX iX, PosY iY);

    bool operator==(const Position& iOther);
    bool operator!=(const Position& iOther);

    int x, y;
};

} // namespace ui
} // namespace memo
