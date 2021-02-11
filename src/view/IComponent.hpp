#pragma once
#include "utils/Structs.hpp"

namespace memo {
namespace ui {

class IComponent
{
public:
    virtual ~IComponent() = default;

    virtual void setParent(IComponent* parent) = 0;
    virtual const IComponent* getParent() const = 0;
    virtual IComponent* getParent() = 0;

    virtual void setHeight(int height) = 0;
    virtual void setWidth(int width) = 0;
    virtual void setSize(const Size& size) = 0;
    virtual int getHeight() const = 0;
    virtual int getWidth() const = 0;
    virtual Size getSize() const = 0;

    virtual void setY(int y) = 0;
    virtual void setAbsY(int y) = 0;
    virtual void setX(int x) = 0;
    virtual void setAbsX(int x) = 0;
    virtual void setPosition(const Position& pos) = 0;
    virtual void setAbsPosition(const Position& pos) = 0;
    virtual int getY() const = 0;
    virtual int getAbsY() const = 0;
    virtual int getX() const = 0;
    virtual int getAbsX() const = 0;
    virtual Position getPosition() const = 0;
    virtual Position getAbsPosition() const = 0;
};

} // namespace ui
} // namespace memo
