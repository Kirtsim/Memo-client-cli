#include "view/MemoCreateView.hpp"
#include "view/widget/TextView.hpp"
#include "tools/Tools.hpp"
#include "ncurses/functions.hpp"
#include "utils/Enums.hpp"

namespace memo {
namespace ui {

MemoCreateView::MemoCreateView(IComponent* parent)
    : MemoCreateView(Size(), Position(), parent)
{
}

MemoCreateView::MemoCreateView(const Size& size, IComponent* parent)
    : MemoCreateView(size, Position(), parent)
{
}

MemoCreateView::MemoCreateView(const Size& size, const Position& position, IComponent* parent)
    : BaseView(size, position, parent)
    , memoTitleText_(std::make_shared<TextView>())
    , memoDescriptionText_(std::make_shared<TextView>())
    , memoTagsText_(std::make_shared<TextView>())
{
    auto textViewWidth = getWidth() * 0.5;
    memoTitleText_->setWidth(textViewWidth);
    memoTitleText_->setHeight(3);
    memoDescriptionText_->setWidth(textViewWidth);
    memoDescriptionText_->setHeight(13);
    memoTagsText_->setWidth(textViewWidth);
    memoTagsText_->setHeight(5);

    memoTitleText_->setY(5);
    tools::Tools::centerComponent(*memoTitleText_, Center::HORIZONTAL, *this);

    memoDescriptionText_->setY(memoTitleText_->getY() + memoTitleText_->getHeight() + 2);
    tools::Tools::centerComponent(*memoDescriptionText_, Center::HORIZONTAL, *this);

    memoTagsText_->setY(memoDescriptionText_->getY() + memoDescriptionText_->getHeight() + 2);
    tools::Tools::centerComponent(*memoTagsText_, Center::HORIZONTAL, *this);

    registerSubView(memoTitleText_);
    registerSubView(memoDescriptionText_);
    registerSubView(memoTagsText_);

    memoTitleText_->setText("Type the memo title here.");
    memoDescriptionText_->setText("Type description here.");
    memoTagsText_->setText("Tags will be displayed here.");
}

MemoCreateView::~MemoCreateView() = default;

void MemoCreateView::refresh()
{
    BaseView::refresh();
}

const std::shared_ptr<TextView>& MemoCreateView::memoTitleTextView()
{
    return memoTitleText_;
}

const std::shared_ptr<TextView>& MemoCreateView::memoDescriptioneTextView()
{
    return memoDescriptionText_;
}

const std::shared_ptr<TextView>& MemoCreateView::memoTagsTextView()
{
    return memoTagsText_;
}

void MemoCreateView::displayContent()
{
    auto position = memoTitleText_->getAbsPosition();
    position.x += 1;
    position.y -= 1;
    curses::PrintText("Title", getWindow(), position);

    position.y = memoDescriptionText_->getAbsY() - 1;
    curses::PrintText("Description", getWindow(), position);

    position.y = memoTagsText_->getAbsY() - 1;
    curses::PrintText("Tags", getWindow(), position);

    position.x = 2;
    position.y = getHeight() - 2;
    curses::PrintText("Press 'q' to go back.", getWindow(), position);
}

} // namespace ui
} // namespace memo
