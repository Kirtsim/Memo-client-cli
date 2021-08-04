#include "view/MemoCreateView.hpp"
#include "view/widget/TextView.hpp"
#include "view/widget/TextEditView.hpp"
#include "tools/Tools.hpp"
#include "ncurses/functions.hpp"
#include "utils/Enums.hpp"

#include <sstream>

namespace memo::ui {

namespace {
    bool isAButton(MemoCreateView::SubView subView);

    void setButtonSelected(bool selected, const std::shared_ptr<TextView>& button);

    void unselectView(const std::shared_ptr<View>& view, const MemoCreateView::SubView type);
} // namespace

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
    , tags_(std::make_shared<TextView>())
    , cancelButton_(std::make_shared<TextView>())
    , confirmButton_(std::make_shared<TextView>())
{
    subViewMapping_[kTitleArea]       = memoTitleTextEditView_;
    subViewMapping_[kDescriptionArea] = memoDescriptionTextEditView_;
    subViewMapping_[kTagsArea]        = tags_;
    subViewMapping_[kConfirmButton]   = confirmButton_;
    subViewMapping_[kCancelButton]    = cancelButton_;

    const int textViewWidth = static_cast<int>(this->getWidth() * 0.5);
    memoTitleTextEditView_->setWidth(textViewWidth);
    memoTitleTextEditView_->setHeight(3);
    memoDescriptionTextEditView_->setWidth(textViewWidth);
    memoDescriptionTextEditView_->setHeight(13);
    tags_->setWidth(textViewWidth);
    tags_->setHeight(5);

    memoTitleTextEditView_->setY(5);
    tools::Tools::centerComponent(*memoTitleTextEditView_, Center::HORIZONTAL, *this);

    memoDescriptionTextEditView_->setY(memoTitleTextEditView_->getY() + memoTitleTextEditView_->getHeight() + 2);
    tools::Tools::centerComponent(*memoDescriptionTextEditView_, Center::HORIZONTAL, *this);

    tags_->setY(memoDescriptionTextEditView_->getY() + memoDescriptionTextEditView_->getHeight() + 2);
    tools::Tools::centerComponent(*tags_, Center::HORIZONTAL, *this);

    confirmButton_->setText(" Confirm ");
    confirmButton_->resizeToText();
    confirmButton_->setY(tags_->getY() + tags_->getHeight() + 2);
    confirmButton_->setX(tags_->getX());

    cancelButton_->setText(" Cancel ");
    cancelButton_->resizeToText();
    cancelButton_->setY(confirmButton_->getY());
    cancelButton_->setX(confirmButton_->getX() + confirmButton_->getWidth() + 2);

    registerSubView(memoTitleTextEditView_);
    registerSubView(memoDescriptionTextEditView_);
    registerSubView(tags_);
    registerSubView(cancelButton_);
    registerSubView(confirmButton_);

    auto border = curses::DefaultBorder();
    memoTitleTextEditView_->setBorder(border);
    memoDescriptionTextEditView_->setBorder(border);
    tags_->setBorder(border);
}

MemoCreateView::~MemoCreateView() = default;

void MemoCreateView::readInput()
{
    auto focusedView = subViewMapping_[subViewInFocus_];

    if (auto editText = std::dynamic_pointer_cast<TextEditView>(focusedView))
    {
        editText->readInput();
        return;
    }

    curses::KeyPad(confirmButton_->getWindow(), ENABLE);
    int key = curses::ReadChar(confirmButton_->getWindow());
    curses::KeyPad(confirmButton_->getWindow(), DISABLE);
    keyFilter_->filterKey(key);
}

void MemoCreateView::registerKeyFilter(const std::shared_ptr<KeyFilter>& keyFilter)
{
    memoTitleTextEditView_->setKeyFilter(keyFilter);
    memoDescriptionTextEditView_->setKeyFilter(keyFilter);
    keyFilter_ = keyFilter;
}

void MemoCreateView::focusSubView(const MemoCreateView::SubView subView)
{
    if (subView == subViewInFocus_ || subView > kNone)
    {
        return;
    }

    if (subViewInFocus_ != kNone)
    {
        auto currentView = subViewMapping_[subViewInFocus_];
        currentView->looseFocus();
        unselectView(currentView, subViewInFocus_);
    }

    if (subView == kNone)
    {
        return;
    }

    auto viewToFocus = subViewMapping_[subView];
    subViewInFocus_ = subView;

    if (subView == kCancelButton || subView == kConfirmButton)
    {
        auto button = std::dynamic_pointer_cast<TextView>(viewToFocus);
        setButtonSelected(true, button);
    }
    viewToFocus->focus();
}

MemoCreateView::SubView MemoCreateView::subViewInFocus() const
{
    return subViewInFocus_;
}

MemoCreateView::SubView MemoCreateView::focusNextSubView()
{
    size_t index = subViewInFocus_ + 1;
    if (index >= kSubViewCount)
    {
        index = 0;
    }

    auto newSubView = static_cast<SubView>(index);
    focusSubView(newSubView);
    return newSubView;
}

MemoCreateView::SubView MemoCreateView::focusPrevSubView()
{
    size_t index = subViewInFocus_ - 1;
    index = std::min(index, kSubViewCount-1);

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

    position.y = tags_->getAbsY() - 1;
    curses::PrintText("Tags", getWindow(), position);

    position.x = 2;
    position.y = getHeight() - 2;
    curses::PrintText("Press 'ESC' to go back.", getWindow(), position);
}

const std::string& MemoCreateView::memoTitle() const
{
    return memoTitleTextEditView_->text();
}

const std::string& MemoCreateView::memoDescription() const
{
    return memoDescriptionTextEditView_->text();
}

void MemoCreateView::displayTagNames(const std::vector<std::string>& tagNames)
{
    if (tagNames.empty())
        return;

    std::stringstream stream;
    stream << "#" << tagNames.front();
    for (size_t i = 1; i < tagNames.size(); ++i)
    {
        stream << ", #" << tagNames[i];
    }
    tags_->setText(stream.str());

    refreshOnRequest();
}

namespace {

    bool isAButton(const MemoCreateView::SubView subView)
    {
        return subView == MemoCreateView::kCancelButton ||
               subView == MemoCreateView::kConfirmButton;
    }

    void setButtonSelected(const bool selected, const std::shared_ptr<TextView>& button)
    {
        if (!button)
            return;

        auto text = button->text();
        char selectionMark = selected ? '_' : ' ';
        if (!text.empty())
        {
            text.front() = selectionMark;
            text.back() = selectionMark;
            button->setText(text);
            button->refresh();
        }
    }

    void unselectView(const std::shared_ptr<View>& view, const MemoCreateView::SubView type)
    {
        if (isAButton(type))
        {
            auto button = std::dynamic_pointer_cast<TextView>(view);
            setButtonSelected(false, button);
        }
    }
} // namespace

} // namespace memo::ui
