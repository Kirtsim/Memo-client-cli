#include "view/BaseComponent.hpp"

namespace memo {
namespace ui {

BaseComponent::BaseComponent() :
    BaseComponent(Size(), Position())
{}

BaseComponent::BaseComponent(const Size& iSize) :
    BaseComponent(iSize, Position())
{}

BaseComponent::BaseComponent(const Size& iSize, const Position& iPosition) :
    size_(iSize), position_(iPosition)
{}

BaseComponent::~BaseComponent() = default;


void BaseComponent::setHeight(int iHeight)
{
    size_.height = iHeight;
}

void BaseComponent::setWidth(int iWidth)
{
    size_.width = iWidth;
}

void BaseComponent::setSize(const Size& iSize)
{
    size_= iSize;
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

void BaseComponent::setPosition(const Position& iPos)
{
    position_ = iPos;
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


