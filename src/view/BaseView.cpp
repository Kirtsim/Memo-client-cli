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

BaseView::BaseView(IComponent* parent) :
    BaseView( curses::ScreenSize(),
              Position(),
              parent )
{}

BaseView::BaseView(const Size& size, IComponent* parent) :
    BaseView(size, Position(), parent)
{}

BaseView::BaseView(const Size& size, const Position& position, IComponent* parent) :
    IView(size, position, parent),
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

    window_->setSize(getSize());
    window_->setPosition(getPosition());

    positionComponents();
    displayContent();

    window_->setWindowBorder(newBorder_);
    window_->redraw();

    for (const auto& subView : subViews_)
    {
        subView->refresh();
    }
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
