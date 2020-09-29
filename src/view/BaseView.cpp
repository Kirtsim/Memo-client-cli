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

#include "view/widget/Text.hpp"
#include <ncursesw/curses.h>

namespace memo {
namespace ui {

const Border BORDER_DEFAULT {
    ACS_HLINE, ACS_VLINE, ACS_HLINE, ACS_VLINE,
    { ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LLCORNER }
};

BaseView::BaseView(IView* iParent) :
    BaseView({ LINES, COLS }, { 0, 0 }, iParent)
{}

BaseView::BaseView(const Size& iSize, IView* iParent) :
    BaseView(iSize, { 0, 0 }, iParent)
{}

BaseView::BaseView(const Size& iSize, const Position& iPosition, IView* iParent) :
    parentView_(iParent),
    width_(iSize.width), height_(iSize.height),
    x_(iPosition.x), y_(iPosition.y),
    border_(BORDER_DEFAULT),
    visible_(true),
    window_(newwin(height_, width_, 0, 0))
{}

BaseView::~BaseView()
{
    if (window_)
    {
        eraseWindow();
        auto* window = window_.release();
        delwin(window);
    }
}

void BaseView::saveState()
{}

void BaseView::refresh()
{
    if (!visible_) return;

    //Size parentSize = getParentSize();
    Position parentPos = getParentPosition();
    const int y = parentPos.y + y_;
    const int x = parentPos.x + x_;

    positionComponents(*window_);
    displayContent(*window_);

    wresize(window_.get(), height_, width_);
    mvwin(window_.get(), y, x);
    box(window_.get(), border_.left, border_.top);

    wrefresh(window_.get());

    for (const auto& subView : subViews_)
    {
        subView->refresh();
    }
}

Size BaseView::getParentSize() const
{
    if (!parentView_)
    {
        Size size;
        size.width = COLS;
        size.height = LINES;
        return size;
    }
    return parentView_->getSize();
}

Position BaseView::getParentPosition() const
{
    if (parentView_) return parentView_->getPosition();
    return { 0, 0 };
}

void BaseView::eraseWindow()
{
    if (!window_) return;
    wborder(window_.get(), ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(window_.get());
    applyBorder();
}

void BaseView::applyBorder()
{
    if (!window_) return;
    wborder(window_.get(),
            border_.left, border_.right,
            border_.top, border_.bottom,
            border_.corner.upperLeft,
            border_.corner.upperRight,
            border_.corner.lowerLeft,
            border_.corner.lowerRight);
}

void BaseView::setVisible(bool iVisible)
{
    visible_ = iVisible;
}

bool BaseView::isVisible() const
{
    return visible_;
}

void BaseView::setHeight(int iHeight)
{
    height_ = iHeight;
}

void BaseView::setWidth(int iWidth)
{
    width_ = iWidth;
}

void BaseView::setSize(const Size& iSize)
{
    setWidth(iSize.width);
    setHeight(iSize.height);
}

int BaseView::getHeight() const
{
    return height_;
}

int BaseView::getWidth() const
{
    return width_;
}

Size BaseView::getSize() const
{
    return { height_, width_ };
}

void BaseView::setY(int iY)
{
    y_ = iY;
}

void BaseView::setX(int iX)
{
    x_ = iX;
}

void BaseView::setPosition(const Position& iPos)
{
    setY(iPos.y);
    setX(iPos.x);
}

int BaseView::getY() const
{
    return y_;
}

int BaseView::getX() const
{
    return x_;
}

Position BaseView::getPosition() const
{
    Position position;
    position.x = x_;
    position.y = y_;
    return position;
}

void BaseView::setParentView(IView* iParent)
{
    parentView_ = iParent;
}

IView* BaseView::getParentView()
{
    return parentView_;
}

void BaseView::setBorder(const Border& iBorder)
{
    border_ = iBorder;
    applyBorder();
}

Border BaseView::getBorder() const
{
    return border_;
}

void BaseView::displayText(const widget::Text& iText)
{
    mvwprintw(window_.get(), iText.getY(), iText.getX(), iText.getText().c_str());
}

Window_t& BaseView::getWindow()
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
void BaseView::displayContent(Window_t& ioWindow) {}
void BaseView::positionComponents(Window_t& ioWindow) {}
#pragma GCC diagnostic pop

} // namespace ui
} // namespace memo
