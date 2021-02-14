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
    size_(curses::CorrectSize(size)), position_(position), parent_(parent)
{}

BaseComponent::~BaseComponent() = default;

void BaseComponent::setParent(IComponent* parent)
{
    if (this != parent)
        parent_ = parent;
}

const IComponent* BaseComponent::getParent() const
{
    return parent_;
}

IComponent* BaseComponent::getParent()
{
    return parent_;
}

void BaseComponent::setHeight(int height)
{
    auto oldSize = size_;
    size_.height = (height > 0 ? height : curses::ScreenHeight());
    onSizeChanged(oldSize, size_);
}

void BaseComponent::setWidth(int width)
{
    auto oldSize = size_;
    size_.width = (width > 0 ? width : curses::ScreenWidth());
    onSizeChanged(oldSize, size_);
}

void BaseComponent::setSize(const Size& size)
{
    auto oldSize = size_;
    size_= curses::CorrectSize(size);
    onSizeChanged(oldSize, size_);
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
    auto oldPos = position_;
    position_.y = y;
    onPositionChanged(oldPos, position_);
}

void BaseComponent::setX(int x)
{
    setAbsX(getParentPosition().x + x);
}

void BaseComponent::setAbsX(int x)
{
    auto oldPos = position_;
    position_.x = x;
    onPositionChanged(oldPos, position_);
}

void BaseComponent::setPosition(const Position& pos)
{
    setX(pos.x);
    setY(pos.y);
}

void BaseComponent::setAbsPosition(const Position& pos)
{
    auto oldPos = position_;
    position_ = pos;
    onPositionChanged(oldPos, position_);
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

#ifdef _MSC_VER
    #pragma warning( push )
    #pragma warning( disable: 4100 )
#elif defined( __GNUC__ ) || ( __clang__ )
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
void BaseComponent::onSizeChanged(const Size& oldSize, const Size& newSize)
{
}

void BaseComponent::onPositionChanged(const Position& oldPos, const Position& newPos)
{
}
#ifdef _MSC_VER
    #pragma warning( pop )
#elif defined( __GNUC__ ) || ( __clang__ )
    #pragma GCC diagnostic pop
#endif

} // namespace ui
} // namespace memo


