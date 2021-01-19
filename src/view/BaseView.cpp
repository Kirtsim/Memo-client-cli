#include "view/BaseView.hpp"
#include "Client.hpp"
#include "manager/ViewManager.hpp"

namespace memo {
namespace view {

BaseView::BaseView(Client& client, const std::shared_ptr<manager::ViewManager>& viewManager) :
    client_(client),
    viewManager_(viewManager)
{}

BaseView::~BaseView() = default;

const std::shared_ptr<manager::ViewManager>& BaseView::getViewManager()
{
    return viewManager_;
}

Client& BaseView::getClient()
{
    return client_;
}

std::string BaseView::readInput()
{
    std::string input;
    std::cin >> input;
    return input;
}

void BaseView::print(const std::string& content) const
{
    std::cout << content;
}

void BaseView::println(const std::string& content) const
{
    std::cout << content << "\n";
}
} // namespace view
} // namespace memo

#include "ncurses/Window.hpp"
#include "ncurses/functions.hpp"
#include "view/widget/Text.hpp"

namespace memo {
namespace ui {

BaseView::BaseView(IView* parent) :
    BaseView( curses::ScreenSize(),
              Position(),
              parent )
{}

BaseView::BaseView(const Size& size, IView* parent) :
    BaseView(size, Position(), parent)
{}

BaseView::BaseView(const Size& size, const Position& position, IView* parent) :
    parentView_(parent),
    newPosition_(position),
    newSize_(size),
    newBorder_(curses::DefaultBorder()),
    visible_(true),
    window_(std::make_shared<curses::Window>(position, size))
{}

BaseView::~BaseView()
{
}

void BaseView::saveState()
{}

void BaseView::refresh()
{
    if (!visible_) return;

    // Reposition relative to the parent
    Position newPosition = getParentPosition();
    newPosition.y += newPosition_.y;
    newPosition.x += newPosition_.x;

    window_->setSize(newSize_);
    window_->setPosition(newPosition);

    positionComponents();
    displayContent();

    window_->setWindowBorder(newBorder_);
    window_->redraw();

    for (const auto& subView : subViews_)
    {
        subView->refresh();
    }
}

Size BaseView::getParentSize() const
{
    if (!parentView_)
        return curses::ScreenSize();
    return parentView_->getSize();
}

Position BaseView::getParentPosition() const
{
    if (parentView_)
        return parentView_->getPosition();
    return Position(); // return 0, 0 coordinates
}

void BaseView::hideWindow()
{
    window_->erase();
}

void BaseView::setVisible(bool visible)
{
    visible_ = visible;
}

bool BaseView::isVisible() const
{
    return visible_;
}

void BaseView::setHeight(int height)
{
    newSize_.height = height;
}

void BaseView::setWidth(int width)
{
    newSize_.width = width;
}

void BaseView::setSize(const Size& size)
{
    newSize_ = size;
}

int BaseView::getHeight() const
{
    return window_->height();
}

int BaseView::getWidth() const
{
    return window_->width();
}

Size BaseView::getSize() const
{
    return window_->size();
}

void BaseView::setY(int y)
{
    newPosition_.y = y;
}

void BaseView::setAbsY(int y)
{
    newPosition_.y = y - getParentPosition().y;
}

void BaseView::setX(int x)
{
    newPosition_.x = x;
}

void BaseView::setAbsX(int x)
{
    newPosition_.x = x - getParentPosition().x;
}

void BaseView::setPosition(const Position& pos)
{
    newPosition_ = pos;
}

void BaseView::setAbsPosition(const Position& pos)
{
    const auto parentPos = getParentPosition();
    newPosition_.x = pos.x - parentPos.x;
    newPosition_.y = pos.y - parentPos.y; 
}

int BaseView::getY() const
{
    return window_->posY();
}

int BaseView::getAbsY() const
{
    return window_->posY();
}

int BaseView::getX() const
{
    return window_->posX();
}

int BaseView::getAbsX() const
{
    return window_->posX();
}

Position BaseView::getPosition() const
{
    return window_->position();
}

Position BaseView::getAbsPosition() const
{
    return window_->position();
}

void BaseView::setParentView(IView* parent)
{
    parentView_ = parent;
}

IView* BaseView::getParentView()
{
    return parentView_;
}

void BaseView::setBorder(const Border& border)
{
    newBorder_ = border;
}

Border BaseView::getBorder() const
{
    return window_->windowBorder();
}

void BaseView::displayText(const widget::Text& text)
{
    curses::PrintText(text.getText(), *window_, text.getPosition());
}

curses::IWindow& BaseView::getWindow()
{
    return *window_;
}

void BaseView::registerSubView(IView::Ptr subView)
{
    subViews_.insert(subView);
}

void BaseView::removeSubView(IView::Ptr subView)
{
    subViews_.erase(subView);
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void BaseView::focus() {}
void BaseView::beforeViewResized() {}
void BaseView::displayContent() {}
void BaseView::positionComponents() {}
#pragma GCC diagnostic pop

} // namespace ui
} // namespace memo
