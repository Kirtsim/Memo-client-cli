#pragma once

namespace memo {

struct Rows
{
    explicit Rows(int value);

    int value;
};

using Height = Rows;

struct Cols
{
    explicit Cols(int value);

    int value;
};

using Width = Cols;

struct PosX
{
    explicit PosX(int value);

    int value;
};

struct PosY
{
    explicit PosY(int value);

    int value;
};

struct Size
{
    Size();
    Size(Height height, Width width);

    bool operator==(const Size& other) const;
    bool operator!=(const Size& other) const;

    int height, width;
};

struct Position
{
    Position();
    Position(PosX x, PosY y);

    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;

    int x, y;
};

struct Border
{
    unsigned int
        top    = ' ',
        right  = ' ',
        bottom = ' ',
        left   = ' ';

    struct Corner
    {
        unsigned int upperLeft  = ' ';
        unsigned int upperRight = ' ';
        unsigned int lowerLeft  = ' ';
        unsigned int lowerRight = ' ';
    } corner;

    static Border NoBorder() { return Border(); }
};


} // namespace memo
