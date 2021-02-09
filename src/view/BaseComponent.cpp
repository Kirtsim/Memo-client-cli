#include "view/BaseComponent.hpp"
#include "ncurses/functions.hpp"

namespace memo {
namespace ui {

BaseComponent::BaseComponent(IComponent* parent) :
    BaseComponent(Size(), Position(), parent)
{}

BaseComponent::BaseComponent(const Size& size, IComponent* parent) :
    BaseComponent(size, Position(), parent)
{}

BaseComponent::BaseComponent(const Size& size, const Position& position, IComponent* parent) :
    size_(size), position_(position), parent_(parent)
{}

BaseComponent::~BaseComponent() = default;

void BaseComponent::setParent(IComponent* parent)
{
    parent_ = parent;
}

const IComponent* BaseComponent::getParent() const
{
    return parent_;
}

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
    setAbsY(getParentPosition().y + y);
}

void BaseComponent::setAbsY(int y)
{
    position_.y = y;
}

void BaseComponent::setX(int x)
{
    setAbsX(getParentPosition().x + x);
}

void BaseComponent::setAbsX(int x)
{
    position_.x = x;
}

void BaseComponent::setPosition(const Position& pos)
{
    setX(pos.x);
    setY(pos.y);
}

void BaseComponent::setAbsPosition(const Position& pos)
{
    setAbsX(pos.x);
    setAbsY(pos.y);
}

int BaseComponent::getY() const
{
    return position_.y - getParentPosition().y;
}

int BaseComponent::getAbsY() const
{
    return position_.y;
}

int BaseComponent::getX() const
{
    return position_.x - getParentPosition().x;
}

int BaseComponent::getAbsX() const
{
    return position_.x;
}

Position BaseComponent::getPosition() const
{
    const auto parentPos = getParentPosition();
    Position pos = position_;
    pos.x -= parentPos.x;
    pos.y -= parentPos.y;
    return pos;
}

Position BaseComponent::getAbsPosition() const
{
    return position_;
}

Size BaseComponent::getParentSize() const
{
    if (parent_)
        return parent_->getSize();
    return curses::ScreenSize();
}

Position BaseComponent::getParentPosition() const
{
    if (parent_)
        return parent_->getAbsPosition();
    return Position(); // return 0, 0 coordinates
}

} // namespace ui
} // namespace memo


