#pragma once
#include "view/IComponent.hpp"

namespace memo {
namespace ui {

class BaseComponent : public IComponent
{
public:
    explicit BaseComponent(IComponent* parent=nullptr);
    explicit BaseComponent(const Size& size, IComponent* parent=nullptr);
    BaseComponent(const Size& size, const Position& position, IComponent* parent=nullptr);
    virtual ~BaseComponent();

    virtual void setParent(IComponent* parent) override;
    virtual const IComponent* getParent() const override;
    virtual IComponent* getParent() override;

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

protected:
    Size getParentSize() const;
    Position getParentPosition() const;

    virtual void onSizeChanged(const Size& oldSize, const Size& newSize);
    virtual void onPositionChanged(const Position& oldPos, const Position& newPos);

private:
    Size size_;
    Position position_;
    IComponent* parent_;
};

} // namespace ui
} // namespace memo
