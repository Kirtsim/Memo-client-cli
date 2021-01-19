#include "view/BaseComponent.hpp"

namespace memo {
namespace ui {

BaseComponent::BaseComponent() :
    BaseComponent(Size(), Position())
{}

BaseComponent::BaseComponent(const Size& size) :
    BaseComponent(size, Position())
{}

BaseComponent::BaseComponent(const Size& size, const Position& position) :
    size_(size), position_(position)
{}

BaseComponent::~BaseComponent() = default;


void BaseComponent::setHeight(int height)
{
    size_.height = height;
}

void BaseComponent::setWidth(int width)
{
    size_.width = width;
}

void BaseComponent::setSize(const Size& size)
{
    size_= size;
}

int BaseComponent::getHeight() const
{
    return size_.height;
}

int BaseComponent::getWidth() const
{
    return size_.width;
}

Size BaseComponent::getSize() const
{
    return size_;
}

void BaseComponent::setY(int y)
{
    position_.y = y;
}

void BaseComponent::setAbsY(int y)
{
    setY(y);
}

void BaseComponent::setX(int x)
{
    position_.x = x;
}

void BaseComponent::setAbsX(int x)
{
    setX(x);
}

void BaseComponent::setPosition(const Position& pos)
{
    position_ = pos;
}

void BaseComponent::setAbsPosition(const Position& pos)
{
    setPosition(pos);
}

int BaseComponent::getY() const
{
    return position_.y;
}

int BaseComponent::getAbsY() const
{
    return getY();
}

int BaseComponent::getX() const
{
    return position_.x;
}

int BaseComponent::getAbsX() const
{
    return getX();
}

Position BaseComponent::getPosition() const
{
    return position_;
}

Position BaseComponent::getAbsPosition() const
{
    return getPosition();
}

} // namespace ui
} // namespace memo


