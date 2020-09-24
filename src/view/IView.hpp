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

namespace memo {
namespace ui {
namespace view {

struct Size
{
    int height, width;
};

struct Position
{
    int y, x;
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

class IView
{
public:
    using Ptr = std::shared_ptr<IView>;
    virtual ~IView() = default;

    virtual void refresh() = 0;
    virtual void saveState() = 0;

    virtual void focus() = 0;

    virtual void setVisible(bool visible) = 0;
    virtual bool isVisible() const = 0;

    virtual void setHeight(int iHeight) = 0;
    virtual void setWidth(int iWidth) = 0;
    virtual void setSize(const Size& iSize) = 0;
    virtual int getHeight() const = 0;
    virtual int getWidth() const = 0;
    virtual Size getSize() const = 0;

    virtual void setPosY(int iY) = 0;
    virtual void setPosX(int iX) = 0;
    virtual void setPosition(const Position& iPos) = 0;
    virtual int getPosY() const = 0;
    virtual int getPosX() const = 0;
    virtual Position getPosition() const = 0;

    virtual void setParentView(const IView::Ptr& iParent) = 0;
    virtual const IView::Ptr& getParentView() = 0;

    virtual void setBorder(const Border& iBorder) = 0;
    virtual Border getBorder() const = 0;
};
} // namespace view
} // namespace ui
} // namespace memo
