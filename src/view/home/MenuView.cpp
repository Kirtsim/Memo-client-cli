#include "view/home/MenuView.hpp"
#include "view/tools/Tools.hpp"

#include <algorithm>
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
    menuWindow_(derwin(&getWindow(), 0, 0, 1, 1)),
    menu_(new_menu(nullptr)),
    menuWindowSize_({ 0, 0 }),
    menuWindowPos_({ 0, 0 }),
    menuWindowLayout_(Rows(0), Cols(0)),
    selectionMark_(""),
    menuItemsChanged_(false)
{
    set_menu_win(menu_.get(), &getWindow());
    set_menu_sub(menu_.get(), menuWindow_.get());

    // Do not show the item's description
    menu_opts_off(menu_.get(), O_SHOWDESC);
}

MenuView::~MenuView()
{
    unpost_menu(menu_.get());
    free_menu(menu_.release());
    freeTagItems(tagItems_);
    delwin(menuWindow_.release());
}

void MenuView::setMenuItems(const std::vector<MenuItem>& iItems)
{
    auto oldTagItems = tagItems_;
    menuItems_ = iItems;
    tagItems_.clear();

    for (const auto& menuItem : menuItems_)
    {
        tagITEM* item = new_item(menuItem.getName().c_str(),
                                 menuItem.getDescription().c_str());
        tagItems_.push_back(item);
    }

    // Add a termination ITEM - a nullptr (required)
    tagItems_.push_back(nullptr);
    set_menu_items(menu_.get(), (tagITEM**) tagItems_.data());
    freeTagItems(oldTagItems);
    menuItemsChanged_ = true;
}

void MenuView::freeTagItems(std::vector<tagITEM*>& ioTagItems)
{
    if (ioTagItems.empty()) return;

    const size_t itemCount = ioTagItems.size() - 1;
    for (size_t i = 0; i < itemCount; ++i)
        free_item(ioTagItems[i]);
}

void MenuView::setLayout(Rows iRows, Cols iCols)
{
    menuWindowLayout_.rows = iRows.value;
    menuWindowLayout_.cols = iCols.value;
}

void MenuView::setSelectionMark(const std::string& iMark)
{
    selectionMark_ = iMark;
}

int MenuView::navigateMenuUp()
{
    return menu_driver(menu_.get(), REQ_UP_ITEM);
}

int MenuView::navigateMenuDown()
{
    return menu_driver(menu_.get(), REQ_DOWN_ITEM);
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

std::pair<bool, MenuItem> MenuView::getSelected() const
{
    std::pair<bool, MenuItem> selection(false, { -1, "", "" });
    auto* selectedItem = current_item(menu_.get());
    if (selectedItem)
    {
        std::string name = item_name(selectedItem);
        auto it = std::find_if(menuItems_.begin(), menuItems_.end(),
                [&name](const MenuItem& menuItem)
                { return name == menuItem.getName(); });
        if (it != std::end(menuItems_))
        {
            selection.first = true;
            selection.second = *it;
        }
    }

    return selection;
}

void MenuView::beforeViewResized()
{
    if (menuChanged())
        unpost_menu(menu_.get());
}

void MenuView::positionComponents(Window_t& ioWindow)
{
    auto* selectedItem = current_item(menu_.get());

    /// Make sure the subwindow is large enough to display all items
    menuWindowSize_ = getMinimumRequiredSize();
    menuWindowPos_.y = 2; menuWindowPos_.x = 1;
    applyMenuChanges();

    set_current_item(menu_.get(), selectedItem);
}

Size MenuView::getMinimumRequiredSize() const
{
    Size size;
    scale_menu(menu_.get(), &size.height, &size.width);
    return size;
}

bool MenuView::menuChanged()
{
    return menuPositionChanged() ||
           menuSizeChanged() ||
           menuMarkerChanged() ||
           menuLayoutChanged() || menuItemsChanged_;
}

bool MenuView::menuPositionChanged()
{
    return menuWindowPos_.x != menuWindow_->_begx || menuWindowPos_.y != menuWindow_->_begy;
}

bool MenuView::menuMarkerChanged()
{
    std::string oldSelectionMark(menu_mark(menu_.get()));
    return oldSelectionMark != selectionMark_;
}

bool MenuView::menuSizeChanged()
{
    const int oldWidth = menuWindow_->_maxy - menuWindow_->_begy;
    const int oldHeight = menuWindow_->_maxx - menuWindow_->_begx;

    return oldWidth != menuWindowSize_.width || oldHeight != menuWindowSize_.height;
}

bool MenuView::menuLayoutChanged()
{
    Layout oldMenuLayout(Rows(0), Cols(0));
    menu_format(menu_.get(), &oldMenuLayout.rows, &oldMenuLayout.cols);
    return menuWindowLayout_ != oldMenuLayout;
}

void MenuView::applyMenuChanges()
{
    if (!menuChanged()) return;

    set_menu_format(menu_.get(), menuWindowLayout_.rows, menuWindowLayout_.cols);
    if (menuSizeChanged())
        wresize(menuWindow_.get(), menuWindowSize_.height, menuWindowSize_.width);
    if (menuPositionChanged())
        mvderwin(menuWindow_.get(), menuWindowPos_.y, menuWindowPos_.x);
    set_menu_mark(menu_.get(), selectionMark_.c_str());
    menuItemsChanged_ = false;

    post_menu(menu_.get());
}

} // namespace ui
} // namespace memo
