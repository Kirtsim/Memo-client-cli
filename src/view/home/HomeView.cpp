#include "view/home/HomeView.hpp"
#include "view/home/MenuView.hpp"
#include "view/widget/TextView.hpp"
#include "view/tools/Tools.hpp"
#include "ncurses/functions.hpp"
#include "utils/Enums.hpp"

namespace memo {
namespace ui {


///////////////////////////////////////////////////////////////
/// HomeView
///////////////////////////////////////////////////////////////
const std::vector<MenuItem> HomeView::kMenuItems {
    MenuItem(E_MenuItem::LIST_MEMOS,  "List Memos", ""),
    MenuItem(E_MenuItem::LIST_TAGS,   "List Tags", ""),
    MenuItem(E_MenuItem::CREATE_MEMO, "Create Memo", ""),
    MenuItem(E_MenuItem::CREATE_TAG,  "Create Tag", ""),
    MenuItem(E_MenuItem::DELETE_MEMO, "Delete Memo", ""),
    MenuItem(E_MenuItem::DELETE_TAG,  "Delete Tag", ""),
    MenuItem(E_MenuItem::EXIT,		  "Exit", ""),
};

HomeView::HomeView(IComponent* parent) :
    HomeView( curses::ScreenSize(),
              Position(),
              parent )
{
}

HomeView::HomeView(const Size& size, IComponent* parent) :
    HomeView(size, Position(), parent)
{}

HomeView::HomeView(const Size& size, const Position& position, IComponent* parent) :
    BaseView(size, position, parent),
    errorStatus_(std::make_unique<TextView>()),
    windowTitle_(std::make_unique<TextView>()),
    menuView_(std::make_shared<MenuView>())
{
    registerSubView(menuView_);
    registerSubView(errorStatus_);
    registerSubView(windowTitle_);
    errorStatus_->setText("SOME TEXT");
    errorStatus_->resizeToText();
    windowTitle_->setText("Welcome to the Memo-client-cli");
    windowTitle_->resizeToText();
    menuView_->setMenuItems(kMenuItems);
    menuView_->setSelectionMark(" * ");
    menuView_->setLayout(Rows(4), Cols(2));
    menuView_->applyMenuChanges();

    registerSubView(errorStatus_);
    registerSubView(windowTitle_);
}

HomeView::~HomeView() = default;

const std::shared_ptr<MenuView>& HomeView::getMenuView()
{
    return menuView_;
}

void HomeView::focus()
{
}

void HomeView::setErrorStatus(const std::string& status)
{
    errorStatus_->setText(status);
}

void HomeView::displayContent()
{
    windowTitle_->setY(2);
    errorStatus_->setY(getHeight() - 2);
    windowTitle_->setWidth(windowTitle_->text().size());
    errorStatus_->setWidth(errorStatus_->text().size());
    tools::Tools::centerComponent(*windowTitle_, Center::HORIZONTAL, *this);
    tools::Tools::centerComponent(*errorStatus_, Center::HORIZONTAL, *this);

    Size minMenuSize = menuView_->getMinimumRequiredSize();
    menuView_->setWidth(minMenuSize.width + 4);
    menuView_->setHeight(minMenuSize.height + 4);
    tools::Tools::centerComponent(*menuView_, Center::CENTER, *this);

}

} // namespace ui
} // namespace memo
