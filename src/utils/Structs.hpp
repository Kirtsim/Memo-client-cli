#pragma once

namespace memo {

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

struct Border
{
    unsigned int top, right, bottom, left;

    struct Corner
    {
        unsigned int upperLeft;
        unsigned int upperRight;
        unsigned int lowerLeft;
        unsigned int lowerRight;
    } corner;
};

extern const Border BORDER_DEFAULT;

} // namespace memo
