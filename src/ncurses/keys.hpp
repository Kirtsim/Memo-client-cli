#pragma once

namespace memo::curses {

class Key
{
public:
    static const int kUp;
    static const int kDown;
    static const int kLeft;
    static const int kRight;

    static const int kEnter;
    static const int kEsc;
    static const int kTab;
    static const int kShiftTab;
    static const int kBackSpace;
    static const int kSpace;

    Key() = delete;
    Key(const Key&) = delete;
    Key& operator=(const Key&) = delete;

};

} // namespace memo::curses
