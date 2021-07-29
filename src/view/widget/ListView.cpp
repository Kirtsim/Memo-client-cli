#include "view/widget/ListView.hpp"
#include "view/tools/Tools.hpp"
#include "ncurses/functions.hpp"

#include <algorithm>
#include <memory>

namespace memo::ui {

namespace {
    const int kTotalBorderWidth = 2;
} // namespace

ListView::ListView(IComponent* parent)
    : ListView(Size(), Position(), parent)
{
}

ListView::ListView(const Size& size, IComponent* parent)
    : ListView(size, Position(), parent)
{
}

ListView::ListView(const Size& size, const Position& position, IComponent* parent)
    : BaseView(size, position, parent)
{
}

ListView::~ListView() = default;

void ListView::setItems(const std::vector<ListItemPtr>& items)
{
    clearDisplayedRows();

    items_ = items;
    selected_ = firstVisItem_ = 0;

    lastVisItem_ = std::min(items.size()-1, maxVisibleItems() -1);
    refreshOnRequest();
}

const std::vector<ListItemPtr>& ListView::items() const
{
    return items_;
}

std::vector<ListItemPtr> ListView::items()
{
    return items_;
}

size_t ListView::itemCount() const
{
    return items_.size();
}

bool ListView::empty() const
{
    return items_.empty();
}

void ListView::select(size_t pos)
{
    if (items_.size() < pos)
        return;
    selected_ = pos;
    refreshOnRequest();
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
    refreshOnRequest();
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
    refreshOnRequest();
    return true;
}

void ListView::setSelectionMark(const std::string& mark)
{
    selectionMark_ = mark;
    refreshOnRequest();
}

const std::string& ListView::selectionMark() const
{
    return selectionMark_;
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
    if (getWidth() - static_cast<int>(selectionMark_.size()) - kTotalBorderWidth <= 0 || items_.empty())
        return;

    std::vector<std::string> textItems;
    for (size_t i = firstVisItem_; i <= lastVisItem_; ++i)
    {
        const auto& item = items_[i];
        if (!item)
            continue;
        
        std::string text = (selected_ == i) ? selectionMark_ : std::string(selectionMark_.size(), ' ');
        text += item->text().substr(0, maxRowChars());
        textItems.emplace_back(text);
    }
    displayTextRows(textItems);
}

size_t ListView::maxVisibleItems() const
{
    const auto height = static_cast<size_t>(getHeight());
    if (height < 3)
        return 0;
    return height - 2;
}

void ListView::displayTextRows(const std::vector<std::string>& textRows)
{
    const size_t xStart = 1;
    int yPos = 1;

    for (const auto& text : textRows)
    {
        const std::string clearRow(maxRowChars(), ' ');
        displayText(clearRow, { PosX(xStart), PosY(yPos) });
        displayText(text, { PosX(xStart), PosY(yPos) });
        ++yPos;
    }
}

void ListView::clearDisplayedRows()
{
    const auto visibleCount = std::min(items_.size(), lastVisItem_ - firstVisItem_ + 1);
    const std::string emptyRow(maxRowChars(), ' ');

    std::vector<std::string> emptyRows(visibleCount, emptyRow);
    displayTextRows(emptyRows);
}

size_t ListView::maxRowChars() const
{
    const int borders = 2;
    const int maxChars = std::max(0, getWidth() - borders - static_cast<int>(selectionMark_.size()));
    return static_cast<size_t>(maxChars);
}

} // namespace memo::ui
