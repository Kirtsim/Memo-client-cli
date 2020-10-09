#include "view/home/MenuView.hpp"

#include <ncursesw/menu.h>

namespace memo {
namespace ui {

const std::vector<std::string> MenuView::kMenuItemNames {
    "List Memo",   "List Tag",
    "Search Memo", "Search Tag",
    "Create Memo", "Create Tag",
};

MenuView::MenuView(IView* iParent) :
    MenuView({ 0, 0 }, { 0, 0 }, iParent)
{}

MenuView::MenuView(const Size& iSize, IView* iParent) :
    MenuView(iSize, { 0, 0 }, iParent)
{}

MenuView::MenuView(const Size& iSize, const Position& iPosition, IView* iParent) :
    BaseView(iSize, iPosition, iParent),
    menuSubWindow_(derwin(&getWindow(), 3, 50, 2, 0))
{
    for (int i = 0; i < MENU_ITEM_COUNT; ++i)
    {
        menuItems_[i] = new_item(kMenuItemNames[i].c_str(), nullptr);
    }
    menuItems_[MENU_ITEM_COUNT] = nullptr; // required

    /// Setup the ncurses menu
    menu_.reset(new_menu((ITEM**)menuItems_.data()));
    set_menu_win(menu_.get(), &getWindow());
    set_menu_sub(menu_.get(), menuSubWindow_.get());
    setLayout(3, 2); // rows, cols
    setSelectionMark(" >> ");

    post_menu(menu_.get());

    // Do not show the item's description
    menu_opts_off(menu_.get(), O_SHOWDESC);

}

MenuView::~MenuView()
{
    unpost_menu(menu_.get());
    free_menu(menu_.release());
    for (int i = 0; i < MENU_ITEM_COUNT; ++i)
        free_item(menuItems_[i]);
    delwin(menuSubWindow_.release());
}

void MenuView::setMenuItems(const std::vector<MenuItem>& iItems)
{
    // TODO: setMenuItems implementation
}

void MenuView::setLayout(int iRows, int iCols)
{
    set_menu_format(menu_.get(), iRows, iCols);
}

void MenuView::setSelectionMark(const std::string& iMark)
{
    set_menu_mark(menu_.get(), iMark.c_str()); // selection mark
}

int MenuView::navigateMenuUp()
{
    menu_driver(menu_.get(), REQ_UP_ITEM);
    return 0;
}

int MenuView::navigateMenuDown()
{
    menu_driver(menu_.get(), REQ_DOWN_ITEM);
    return 0;
}

int MenuView::navigateMenuLeft()
{
    menu_driver(menu_.get(), REQ_LEFT_ITEM);
    return 0;
}

int MenuView::navigateMenuRight()
{
    menu_driver(menu_.get(), REQ_RIGHT_ITEM);
    return 0;
}

int MenuView::getSelected() const
{
    return 0;
}

void MenuView::positionComponents(Window_t& ioWindow)
{
    /// Make sure the subwindow is large enought to display all items
    Size minSize = getMinimumRequiredSize();
    wresize(menuSubWindow_.get(), minSize.height, minSize.width);
}

Size MenuView::getMinimumRequiredSize() const
{
    Size size;
    scale_menu(menu_.get(), &size.height, &size.width);
    return size;
}

} // namespace ui
} // namespace memo
