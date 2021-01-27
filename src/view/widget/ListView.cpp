#include "view/widget/ListView.hpp"
#include "view/widget/Text.hpp"
#include "view/tools/Tools.hpp"
#include "ncurses/functions.hpp"

#include <algorithm>
#include <memory>

namespace memo {
namespace ui {

ListView::ListView(IComponent* parent)
    : ListView(Size(), Position(), parent)
{
    auto size = curses::ScreenSize();
    size.height /= 2;
    size.width /= 2;
    setSize(size);

    auto pos = getParentPosition();
    auto siz = getParentSize();
    tools::Bounds bounds;
    bounds.startX = pos.x;
    bounds.startY = pos.y;
    bounds.endX = pos.x + siz.width;
    bounds.endY = pos.y + siz.height;
    tools::Tools::centerComponent(*this, tools::Center::HORIZONTAL | tools::Center::VERTICAL, bounds);
}

ListView::ListView(const Size& size, IComponent* parent)
    : ListView(size, Position(), parent)
{
}

ListView::ListView(const Size& size, const Position& position, IComponent* parent)
    : BaseView(size, position, parent)
{
}

void ListView::setItems(const std::vector<ListItemPtr>& items)
{
    items_ = items;
    selected_ = firstVisItem_ = 0;

    lastVisItem_ = std::min(items.size()-1, maxVisibleItems() -1);
}

const std::vector<ListItemPtr>& ListView::items() const
{
    return items_;
}

size_t ListView::itemCount() const
{
    return items_.size();
}

void ListView::select(size_t pos)
{
    if (items_.size() < pos)
        return;
    selected_ = pos;
}

bool ListView::selectNext()
{
    if (selected_ + 1 == items_.size())
        return false;

    ++selected_;
    if (lastVisItem_ < selected_)
    {
        ++firstVisItem_;
        ++lastVisItem_;
    }
    return true;
}

bool ListView::selectPrev()
{
    if (selected_ == 0)
        return false;
    --selected_;
    if (firstVisItem_ > selected_)
    {
        --firstVisItem_;
        --lastVisItem_;
    }
    return true;
}

ListItemPtr ListView::selected() const
{
    if (items_.empty())
        return nullptr;
    return items_[selected_];
}

size_t ListView::selectedPos() const
{
    return selected_;
}

void ListView::displayContent()
{
    if (getWidth() - 4 <= 0)
        return;
    const size_t maxChars = getWidth() - 4;
    const size_t xStart = 1;
    const size_t xEnd = getWidth(); 
    size_t yPos = 1;
    
    for (size_t i = firstVisItem_; i <= lastVisItem_; ++i)
    {
        const auto& item = items_[i];
        if (!item)
            continue;
        
        std::string text = (selected_ == i) ? "* " : "  ";
        text += item->text().substr(0, maxChars);

        curses::PrintText(text, getWindow(), { PosX(xStart), PosY(yPos) });
        ++yPos;
    }
}

size_t ListView::maxVisibleItems() const
{
    const int height = getHeight();
    if (height < 3)
        return 0;
    return getHeight() - 2;
}

} // namespace ui
} // namespace memo
