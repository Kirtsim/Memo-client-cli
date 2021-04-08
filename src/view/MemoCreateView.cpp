#include "view/MemoCreateView.hpp"
#include "view/widget/TextView.hpp"
#include "view/widget/TextEditView.hpp"
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
    , memoTitleTextEditView_(std::make_shared<TextEditView>())
    , memoDescriptionTextEditView_(std::make_shared<TextEditView>())
    , memoTagsTextEditView_(std::make_shared<TextEditView>())
{
    auto textViewWidth = getWidth() * 0.5;
    memoTitleTextEditView_->setWidth(textViewWidth);
    memoTitleTextEditView_->setHeight(3);
    memoDescriptionTextEditView_->setWidth(textViewWidth);
    memoDescriptionTextEditView_->setHeight(13);
    memoTagsTextEditView_->setWidth(textViewWidth);
    memoTagsTextEditView_->setHeight(5);

    memoTitleTextEditView_->setY(5);
    tools::Tools::centerComponent(*memoTitleTextEditView_, Center::HORIZONTAL, *this);

    memoDescriptionTextEditView_->setY(memoTitleTextEditView_->getY() + memoTitleTextEditView_->getHeight() + 2);
    tools::Tools::centerComponent(*memoDescriptionTextEditView_, Center::HORIZONTAL, *this);

    memoTagsTextEditView_->setY(memoDescriptionTextEditView_->getY() + memoDescriptionTextEditView_->getHeight() + 2);
    tools::Tools::centerComponent(*memoTagsTextEditView_, Center::HORIZONTAL, *this);

    registerSubView(memoTitleTextEditView_);
    registerSubView(memoDescriptionTextEditView_);
    registerSubView(memoTagsTextEditView_);

    auto border = curses::DefaultBorder();
    memoTitleTextEditView_->setBorder(border);
    memoDescriptionTextEditView_->setBorder(border);
    memoTagsTextEditView_->setBorder(border);
}

MemoCreateView::~MemoCreateView() = default;

void MemoCreateView::refresh()
{
    BaseView::refresh();
}

const std::shared_ptr<TextEditView>& MemoCreateView::memoTitleTextEditView()
{
    return memoTitleTextEditView_;
}

const std::shared_ptr<TextEditView>& MemoCreateView::memoDescriptionTextEditView()
{
    return memoDescriptionTextEditView_;
}

const std::shared_ptr<TextEditView>& MemoCreateView::memoTagsTextEditView()
{
    return memoTagsTextEditView_;
}

void MemoCreateView::displayContent()
{
    auto position = memoTitleTextEditView_->getAbsPosition();
    position.x += 1;
    position.y -= 1;
    curses::PrintText("Title", getWindow(), position);

    position.y = memoDescriptionTextEditView_->getAbsY() - 1;
    curses::PrintText("Description", getWindow(), position);

    position.y = memoTagsTextEditView_->getAbsY() - 1;
    curses::PrintText("Tags", getWindow(), position);

    position.x = 2;
    position.y = getHeight() - 2;
    curses::PrintText("Press 'ESC' to go back.", getWindow(), position);
}

} // namespace ui
} // namespace memo
