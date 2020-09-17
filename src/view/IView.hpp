#pragma once

namespace memo {
namespace view {

class IView
{
public:
    virtual ~IView() = default;

    virtual void display() = 0;
};
} // namespace view
} // namespace memo
