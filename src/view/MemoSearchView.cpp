#include "view/MemoSearchView.hpp"
#include "view/widget/ListView.hpp"
#include "view/widget/TextView.hpp"
#include "ncurses/functions.hpp"
#include "tools/Tools.hpp"
#include "utils/Enums.hpp"

namespace memo::ui {

MemoSearchView::MemoSearchView(IComponent* parent)
    : MemoSearchView(Size(), Position(), parent)
{
}

MemoSearchView::MemoSearchView(const Size& size, IComponent* parent)
    : MemoSearchView(size, Position(), parent)
{
}

MemoSearchView::MemoSearchView(const Size& size, const Position& position, IComponent* parent)
    : BaseView(size, position, parent)
    , memoListView_(std::make_shared<ListView>(this))
{
    registerSubView(memoListView_);

    memoListView_->setWidth(size.width * 0.5);
    memoListView_->setHeight(size.height * 0.5);
    tools::CenterComponent(*memoListView_, Center::CENTER, *this);
}

const ListViewPtr& MemoSearchView::memoListView()
{
    return memoListView_;
}

void MemoSearchView::displayContent()
{
    Position textPos;
    textPos.x = memoListView_->getX();
    textPos.y = memoListView_->getY() + memoListView_->getHeight() + 1;
    displayText("Press 'q' to return to menu", textPos);
}

void MemoSearchView::refresh()
{
    BaseView::refresh();
}
} // namespace memo::ui
