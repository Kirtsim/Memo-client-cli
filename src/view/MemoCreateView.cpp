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
    , cancelButton_(std::make_shared<TextView>())
    , confirmButton_(std::make_shared<TextView>())
{
    subViewMapping_[kTitleArea] = memoTitleTextEditView_;
    subViewMapping_[kDescriptionArea] = memoDescriptionTextEditView_;
    subViewMapping_[kTagsArea] = memoTagsTextEditView_;
    subViewMapping_[kConfirmButton] = confirmButton_;
    subViewMapping_[kCancelButton] = cancelButton_;

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

    confirmButton_->setText(" Confirm ");
    confirmButton_->resizeToText();
    confirmButton_->setY(memoTagsTextEditView_->getY() + memoTagsTextEditView_->getHeight() + 2);
    confirmButton_->setX(memoTagsTextEditView_->getX());

    cancelButton_->setText(" Cancel ");
    cancelButton_->resizeToText();
    cancelButton_->setY(confirmButton_->getY());
    cancelButton_->setX(confirmButton_->getX() + confirmButton_->getWidth() + 2);

    registerSubView(memoTitleTextEditView_);
    registerSubView(memoDescriptionTextEditView_);
    registerSubView(memoTagsTextEditView_);
    registerSubView(cancelButton_);
    registerSubView(confirmButton_);

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

void MemoCreateView::readInput()
{
    if ((subViewInFocus_ >= kSubViewCount ||
         subViewInFocus_ == kCancelButton ||
         subViewInFocus_ == kConfirmButton) && keyFilter_)
    {
        curses::KeyPad(confirmButton_->getWindow(), ENABLE);
        int key = curses::ReadChar(confirmButton_->getWindow());
        curses::KeyPad(confirmButton_->getWindow(), DISABLE);
        keyFilter_->filterKey(key);
    }
    else
    {
        auto editView = std::dynamic_pointer_cast<TextEditView>(subViewMapping_[subViewInFocus_]);
        if (editView)
        {
            editView->readInput();
        }
    }
}

void MemoCreateView::registerKeyFilter(const std::shared_ptr<KeyFilter>& keyFilter)
{
    memoTitleTextEditView_->setKeyFilter(keyFilter);
    memoDescriptionTextEditView_->setKeyFilter(keyFilter);
    memoTagsTextEditView_->setKeyFilter(keyFilter);
    keyFilter_ = keyFilter;
}

void MemoCreateView::focusSubView(const MemoCreateView::SubView subView)
{
    if (subView >= kSubViewCount)
    {
        return;
    }

    auto viewToFocus = subViewMapping_[subView];
    unfocusSubView(subViewInFocus_);
    subViewInFocus_ = subView;

    if (subView == kCancelButton || subView == kConfirmButton)
    {
        auto textView = std::dynamic_pointer_cast<TextView>(viewToFocus);
        auto text = textView->text();
        if (!text.empty())
        {
            text.front() = '_';
            text.back() = '_';
            textView->setText(text);
            textView->refresh();
        }
        // change the view background colour or sth
    }
    viewToFocus->focus();
}

void MemoCreateView::unfocusSubView(MemoCreateView::SubView subView)
{
    if (subView >= kSubViewCount)
    {
        return;
    }
    auto viewToFocus = subViewMapping_[subViewInFocus_];
    if (subView == kCancelButton || subView == kConfirmButton)
    {
        auto textView = std::dynamic_pointer_cast<TextView>(viewToFocus);
        auto text = textView->text();
        if (!text.empty())
        {
            text.front() = ' ';
            text.back() = ' ';
            textView->setText(text);
            textView->refresh();
        }
        // change the view background colour or sth
    }

    auto viewToUnfocus = subViewMapping_[subView];
    viewToUnfocus->looseFocus();
    subViewInFocus_ = SubView::kNone;
}

MemoCreateView::SubView MemoCreateView::subViewInFocus() const
{
    return subViewInFocus_;
}

MemoCreateView::SubView MemoCreateView::focusNextSubView()
{
    auto index = static_cast<size_t>(subViewInFocus_) + 1;
    if (index >= subViewMapping_.size())
    {
        index = 0;
    }
    auto newSubView = static_cast<SubView>(index);
    focusSubView(newSubView);
    return newSubView;
}

MemoCreateView::SubView MemoCreateView::focusPrevSubView()
{
    auto index = static_cast<size_t>(subViewInFocus_) - 1;
    if (subViewInFocus_ == kNone)
    {
        index = 0;
    }
    index = std::min(index, subViewMapping_.size()-1);
    auto newSubView = static_cast<SubView>(index);
    focusSubView(newSubView);
    return newSubView;
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
