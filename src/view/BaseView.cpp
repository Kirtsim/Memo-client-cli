#include "view/BaseView.hpp"
#include "Client.hpp"
#include "manager/ViewManager.hpp"

namespace memo {
namespace view {

BaseView::BaseView(Client& iClient, const std::shared_ptr<manager::ViewManager>& iViewManager) :
    client_(iClient),
    viewManager_(iViewManager)
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

void BaseView::print(const std::string& iContent) const
{
    std::cout << iContent;
}

void BaseView::println(const std::string& iContent) const
{
    std::cout << iContent << "\n";
}
} // namespace view
} // namespace memo

#include "ncurses/Window.hpp"
#include "ncurses/functions.hpp"
#include "view/widget/Text.hpp"

namespace memo {
namespace ui {

BaseView::BaseView(IView* iParent) :
    BaseView( curses::ScreenSize(),
              Position(),
              iParent )
{}

BaseView::BaseView(const Size& iSize, IView* iParent) :
    BaseView(iSize, Position(), iParent)
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

    beforeViewResized();
    window_->setSize(newSize_);
    window_->setPosition(newPosition);

    positionComponents(*window_);
    displayContent(*window_);

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

void BaseView::setVisible(bool iVisible)
{
    visible_ = iVisible;
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

void BaseView::setX(int x)
{
    newPosition_.x = x;
}

void BaseView::setPosition(const Position& pos)
{
    newPosition_ = pos;
}

int BaseView::getY() const
{
    return window_->posY();
}

int BaseView::getX() const
{
    return window_->posX();
}

Position BaseView::getPosition() const
{
    return window_->position();
}

void BaseView::setParentView(IView* iParent)
{
    parentView_ = iParent;
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

void BaseView::displayText(const widget::Text& iText)
{
    curses::PrintText(iText.getText(), *window_, iText.getPosition());
}

curses::IWindow& BaseView::getWindow()
{
    return *window_;
}

void BaseView::registerSubView(IView::Ptr iSubView)
{
    subViews_.insert(iSubView);
}

void BaseView::removeSubView(IView::Ptr iSubView)
{
    subViews_.erase(iSubView);
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void BaseView::focus() {}
void BaseView::beforeViewResized() {}
void BaseView::displayContent(curses::IWindow& ioWindow) {}
void BaseView::positionComponents(curses::IWindow& ioWindow) {}
#pragma GCC diagnostic pop

} // namespace ui
} // namespace memo
