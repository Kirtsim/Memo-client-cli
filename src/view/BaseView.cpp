#include "view/BaseView.hpp"
#include "ncurses/Window.hpp"
#include "ncurses/functions.hpp"
#include "view/widget/TextView.hpp"

namespace memo {
namespace ui {

BaseView::BaseView(IComponent* parent) :
    BaseView( curses::ScreenSize(),
              Position(),
              parent )
{}

BaseView::BaseView(const Size& size, IComponent* parent) :
    BaseView(size, Position(), parent)
{}

BaseView::BaseView(const Size& size, const Position& position, IComponent* parent) :
    View(size, position, parent),
    newBorder_(curses::DefaultBorder()),
    visible_(true),
    window_(std::make_shared<curses::Window>(position, size))
{}

BaseView::~BaseView()
{
}

void BaseView::saveState()
{
}

void BaseView::refresh()
{
    if (!visible_) return;

    if (needsRefresh_)
    {
        window_->setSize(getSize());
        window_->setPosition(getAbsPosition());

        positionComponents();
        displayContent();

        window_->setWindowBorder(newBorder_);

        printForeground();

        window_->redraw();
        for (const auto& subView : subViews_)
        {
            subView->refreshOnRequest();
        }
    }

    for (const auto& subView : subViews_)
    {
        subView->refresh();
    }
    needsRefresh_ = false;
}

void BaseView::refreshOnRequest()
{
    needsRefresh_ = true;
}

void BaseView::setVisible(bool visible)
{
    visible_ = visible;
}

bool BaseView::isVisible() const
{
    return visible_;
}

void BaseView::setBorder(const Border& border)
{
    newBorder_ = border;
}

Border BaseView::getBorder() const
{
    return newBorder_;
}

void BaseView::displayText(const std::string& text, const Position& position)
{
    curses::PrintText(text, *window_, position);
}

curses::IWindow& BaseView::getWindow()
{
    return *window_;
}

void BaseView::registerSubView(View::Ptr subView)
{
    if (!subView || this == subView.get())
        return;
    subViews_.insert(subView);
    subView->setParent(this);
}

void BaseView::removeSubView(View::Ptr subView)
{
    if (!subView)
        return;
    subViews_.erase(subView);
    if (subView->getParent() == this)
        subView->setParent(nullptr);
}

Size BaseView::getParentSize() const
{
    auto parent = getParent();
    if (parent)
        return parent->getSize();

    return curses::ScreenSize();
}

Position BaseView::getParentPosition() const
{
    auto parent = getParent();
    if (parent)
        return parent->getAbsPosition();

    return {}; // x=0, y=0
}

void BaseView::onSizeChanged(const Size& oldSize, const Size& newSize)
{
    if (oldSize != newSize)
    {
        refreshOnRequest();
        parentRequestOnRefresh();
    }
}

void BaseView::onPositionChanged(const Position& oldPos, const Position& newPos)
{
    if (oldPos != newPos)
    {
        refreshOnRequest();
        parentRequestOnRefresh();
    }
}

void BaseView::parentRequestOnRefresh()
{
    auto parentComponent = getParent();
    if (auto parentView = dynamic_cast<View*>(parentComponent))
        parentView->refreshOnRequest();
}

void BaseView::focus() {}
void BaseView::beforeViewResized() {}
void BaseView::displayContent() {}
void BaseView::positionComponents() {}
void BaseView::printForeground() {}

} // namespace ui
} // namespace memo
