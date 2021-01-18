#include "view/home/MenuView.hpp"
#include "view/tools/Tools.hpp"
#include "ncurses/SubWindow.hpp"

#include <memory>
#include <algorithm>
#include <menu.h>

namespace memo {
namespace ui {

const std::vector<std::string> MenuView::kMenuItemNames {
    "List Memo",   "List Tag",
    "Search Memo", "Search Tag",
    "Create Memo", "Create Tag",
};

MenuView::MenuView(IView* parent) :
    MenuView(Size(), Position(), parent)
{}

MenuView::MenuView(const Size& size, IView* parent) :
    MenuView(size, Position(), parent)
{}

MenuView::MenuView(const Size& size, const Position& position, IView* parent) :
    BaseView(size, position, parent),
    menuWindow_(std::make_unique<curses::SubWindow>(getWindow(), Position( PosX(1), PosY(1) ))),
    menu_(new_menu(nullptr)),
    menuWindowSize_(Size()),
    menuWindowPos_(Position()),
    menuWindowLayout_(Rows(0), Cols(0)),
    selectionMark_(""),
    menuItemsChanged_(false)
{
    set_menu_win(menu_.get(), getWindow().cursesWindow());
    set_menu_sub(menu_.get(), menuWindow_->cursesWindow());
    // Do not show the item's description
    menu_opts_off(menu_.get(), O_SHOWDESC);
}

MenuView::~MenuView()
{
    unpost_menu(menu_.get());
    free_menu(menu_.release());
    freeTagItems(tagItems_);
}

void MenuView::setMenuItems(const std::vector<MenuItem>& items)
{
    auto oldTagItems = tagItems_;
    menuItems_ = items;
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

void MenuView::freeTagItems(std::vector<tagITEM*>& tagItems)
{
    if (tagItems.empty()) return;

    const size_t itemCount = tagItems.size() - 1;
    for (size_t i = 0; i < itemCount; ++i)
        free_item(tagItems[i]);
}

void MenuView::setLayout(Rows rows, Cols cols)
{
    menuWindowLayout_.rows = rows.value;
    menuWindowLayout_.cols = cols.value;
}

void MenuView::setSelectionMark(const std::string& mark)
{
    selectionMark_ = mark;
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
    auto selectedItem = current_item(menu_.get());
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

void MenuView::positionComponents()
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
    return menuWindow_->position() != menuWindowPos_;
}

bool MenuView::menuMarkerChanged()
{
    std::string oldSelectionMark(menu_mark(menu_.get()));
    return oldSelectionMark != selectionMark_;
}

bool MenuView::menuSizeChanged()
{
    return menuWindow_->size() != menuWindowSize_;
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

    unpost_menu(menu_.get());
    set_menu_format(menu_.get(), menuWindowLayout_.rows, menuWindowLayout_.cols);
    if (menuSizeChanged())
        menuWindow_->setSize(menuWindowSize_);
    if (menuPositionChanged())
        menuWindow_->setPosition(menuWindowPos_);
    set_menu_mark(menu_.get(), selectionMark_.c_str());
    menuItemsChanged_ = false;

    post_menu(menu_.get());
}

MenuView::Layout::Layout(Rows rows, Cols cols) :
    rows(rows.value), cols(cols.value)
{
}

bool MenuView::Layout::operator==(const Layout& other) const
{
    return rows == other.rows && cols == other.cols;
}

bool MenuView::Layout::operator!=(const Layout& other) const
{
    return rows != other.rows || cols != other.cols;
}


} // namespace ui
} // namespace memo
