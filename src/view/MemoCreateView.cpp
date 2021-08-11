#include "view/MemoCreateView.hpp"
#include "view/widget/TextView.hpp"
#include "view/widget/ButtonView.hpp"
#include "view/widget/TextEditView.hpp"
#include "tools/Tools.hpp"
#include "ncurses/functions.hpp"
#include "ncurses/keys.hpp"
#include "utils/Enums.hpp"

#include <sstream>

namespace memo::ui {

namespace {
    void selectView(const std::shared_ptr<View>& view);

    void unselectView(const std::shared_ptr<View>& view);
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
    , tagsAreaHintLabel_(std::make_shared<TextView>())
    , cancelButton_(std::make_shared<ButtonView>())
    , confirmButton_(std::make_shared<ButtonView>())
{
    subViewMapping_[kTitleArea]       = memoTitleTextEditView_;
    subViewMapping_[kDescriptionArea] = memoDescriptionTextEditView_;
    subViewMapping_[kTagsArea]        = tags_;
    subViewMapping_[kConfirmButton]   = confirmButton_;
    subViewMapping_[kCancelButton]    = cancelButton_;

    memoTitleTextEditView_->setBorder(curses::DefaultBorder());
    memoDescriptionTextEditView_->setBorder(curses::DefaultBorder());
    tags_->setBorder(curses::DefaultBorder());

    tagsAreaHintLabel_->setText("Press <Enter> to modify.");
    tagsAreaHintLabel_->resizeToText();

    const std::vector<int> clickOnKeys { curses::Key::kEnter, curses::Key::kSpace };
    confirmButton_->setText(" Confirm ");
    confirmButton_->setBorder(curses::DefaultBorder());
    confirmButton_->resizeToText();
    confirmButton_->listenToKeys(clickOnKeys);
    confirmButton_->setOnButtonClicked([&](int/* key*/) {
        if (onConfirmButtonClicked_)
            onConfirmButtonClicked_();
    });

    cancelButton_->setText(" Cancel ");
    cancelButton_->setBorder(curses::DefaultBorder());
    cancelButton_->resizeToText();
    cancelButton_->listenToKeys(clickOnKeys);
    cancelButton_->setOnButtonClicked([&](int/* key*/) {
        if (onCancelButtonClicked_)
            onCancelButtonClicked_();
    });

    registerSubView(memoTitleTextEditView_);
    registerSubView(memoDescriptionTextEditView_);
    registerSubView(tags_);
    registerSubView(tagsAreaHintLabel_);
    registerSubView(cancelButton_);
    registerSubView(confirmButton_);

    layoutComponents();
}

MemoCreateView::~MemoCreateView() = default;

void MemoCreateView::layoutComponents()
{
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

    tagsAreaHintLabel_->setY(tags_->getY() + tags_->getHeight());
    tagsAreaHintLabel_->setX(tags_->getX() + tags_->getWidth() - tagsAreaHintLabel_->getWidth());

    confirmButton_->setY(tags_->getY() + tags_->getHeight() + 2);
    confirmButton_->setX(tags_->getX());

    cancelButton_->setY(confirmButton_->getY());
    cancelButton_->setX(confirmButton_->getX() + confirmButton_->getWidth() + 2);
}

void MemoCreateView::readInput()
{
    focus();
    while (hasFocus())
    {
        auto focusedView = subViewMapping_[subViewInFocus_];
        focusedView->readInput();
    }
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
        unselectView(currentView);
    }

    if (subView == kNone)
    {
        return;
    }

    auto viewToFocus = subViewMapping_[subView];
    selectView(viewToFocus);
    subViewInFocus_ = subView;

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
void MemoCreateView::doOnConfirmButtonClicked(const std::function<void()>& functionCall)
{
    onConfirmButtonClicked_ = functionCall;
}

void MemoCreateView::doOnCancelButtonClicked(const std::function<void()>& functionCall)
{
    onCancelButtonClicked_ = functionCall;
}

void MemoCreateView::displayContent()
{
    layoutComponents();
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

void MemoCreateView::onKeyFilterSet(const std::function<bool(int)>& filterFunction)
{
    memoTitleTextEditView_->setKeyFilter(filterFunction);
    memoDescriptionTextEditView_->setKeyFilter(filterFunction);
    tags_->setKeyFilter(filterFunction);
    confirmButton_->setKeyFilter(filterFunction);
    cancelButton_->setKeyFilter(filterFunction);
}

namespace {

    void selectView(const std::shared_ptr<View>& view)
    {
        if (view)
        {
            auto border = curses::DefaultBorder();
            border.bottom = border.top = '-';
            border.left = border.right = ':';
            view->setBorder(border);
            view->refreshOnRequest();
            view->refresh();
        }
    }

    void unselectView(const std::shared_ptr<View>& view)
    {
        if (view)
        {
            view->setBorder(curses::DefaultBorder());
            view->refreshOnRequest();
            view->refresh();
        }
    }
} // namespace

} // namespace memo::ui
