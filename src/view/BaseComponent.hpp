#pragma once
#include "view/IComponent.hpp"

namespace memo {
namespace ui {

class BaseComponent : public IComponent
{
public:
    BaseComponent();
    explicit BaseComponent(const Size& size);
    BaseComponent(const Size& size, const Position& position);
    virtual ~BaseComponent();

    virtual void setHeight(int height) override;
    virtual void setWidth(int width) override;
    virtual void setSize(const Size& size) override;
    virtual int getHeight() const override;
    virtual int getWidth() const override;
    virtual Size getSize() const override;

    virtual void setY(int iY) override;
    virtual void setAbsY(int iY) override;
    virtual void setX(int iX) override;
    virtual void setAbsX(int iX) override;
    virtual void setPosition(const Position& pos) override;
    virtual void setAbsPosition(const Position& pos) override;
    virtual int getY() const override;
    virtual int getAbsY() const override;
    virtual int getX() const override;
    virtual int getAbsX() const override;
    virtual Position getPosition() const override;
    virtual Position getAbsPosition() const override;

private:
    Size size_;
    Position position_;
};

} // namespace ui
} // namespace memo
