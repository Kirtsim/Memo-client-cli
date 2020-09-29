#pragma once
#include <memory>

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

#include "view/IComponent.hpp"

namespace memo {
namespace ui {

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

class IView : public IComponent
{
public:
    using Ptr = std::shared_ptr<IView>;

    virtual ~IView() = default;

    virtual void refresh() = 0;
    virtual void saveState() = 0;

    virtual void focus() = 0;

    virtual void setVisible(bool visible) = 0;
    virtual bool isVisible() const = 0;

    virtual void setParentView(IView* iParent) = 0;
    virtual IView* getParentView() = 0;

    virtual void setBorder(const Border& iBorder) = 0;
    virtual Border getBorder() const = 0;
};

} // namespace ui
} // namespace memo
