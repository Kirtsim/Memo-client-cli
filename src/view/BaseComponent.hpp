#pragma once
#include "view/IComponent.hpp"

namespace memo {
namespace ui {

class BaseComponent : public IComponent
{
public:
    BaseComponent();
    explicit BaseComponent(const Size& iSize);
    BaseComponent(const Size& iSize, const Position& iPosition);
    virtual ~BaseComponent();

    virtual void setHeight(int iHeight) override;
    virtual void setWidth(int iWidth) override;
    virtual void setSize(const Size& iSize) override;
    virtual int getHeight() const override;
    virtual int getWidth() const override;
    virtual Size getSize() const override;

    virtual void setY(int iY) override;
    virtual void setX(int iX) override;
    virtual void setPosition(const Position& iPos) override;
    virtual int getY() const override;
    virtual int getX() const override;
    virtual Position getPosition() const override;

private:
    Size size_;
    Position position_;
};

} // namespace ui
} // namespace memo


