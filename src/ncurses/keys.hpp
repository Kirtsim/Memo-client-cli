#pragma once

namespace memo {
namespace curses {

class Key
{
public:
    static const int kUp;
    static const int kDown;
    static const int kLeft;
    static const int kRight;

    static const int kEnter;


    Key() = delete;
    Key(const Key&) = delete;
    Key& operator=(const Key&) = delete;

};

} // namespace curses
} // namespace memo
