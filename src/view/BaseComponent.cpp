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

void BaseComponent::setY(int iY)
{
    position_.y = iY;
}

void BaseComponent::setX(int iX)
{
    position_.x = iX;
}

void BaseComponent::setPosition(const Position& pos)
{
    position_ = pos;
}

int BaseComponent::getY() const
{
    return position_.y;
}

int BaseComponent::getX() const
{
    return position_.x;
}

Position BaseComponent::getPosition() const
{
    return position_;
}


} // namespace ui
} // namespace memo


