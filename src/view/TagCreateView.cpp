#include "view/TagCreateView.hpp"
#include "view/widget/TextEditView.hpp"
#include "view/widget/TextView.hpp"
#include "view/widget/ButtonView.hpp"
#include "view/tools/ViewFocusOperator.hpp"
#include "view/tools/Tools.hpp"
#include "ncurses/functions.hpp"
#include "ncurses/keys.hpp"

namespace memo::ui {

namespace {
    Border SelectionBorder();
}

class TagNameChangedListener : public OnTextChangedListener
{
public:
    ~TagNameChangedListener() override = default;

    void onTextChanged(const std::string& text) override;

    void setCallback(const std::function<void(const std::string&)>& callback);

private:
    std::function<void(const std::string&)> callback_;
};


TagCreateView::TagCreateView(IComponent* parent)
    : TagCreateView(Size(), Position(), parent)
{
}

TagCreateView::TagCreateView(const Size& size, IComponent* parent)
    : TagCreateView(size, Position(), parent)
{
}

TagCreateView::TagCreateView(const Size& size, const Position& position, IComponent* parent)
    : BaseView(size, position, parent)
    , nameEditText_(std::make_shared<TextEditView>(this))
    , infoText_(std::make_shared<TextView>(this))
    , confirmButton_(std::make_shared<ButtonView>(this))
    , cancelButton_(std::make_shared<ButtonView>(this))
    , focusOperator_(std::make_unique<ViewFocusOperator>())
    , tagNameChangedListener_(std::make_shared<TagNameChangedListener>())
{
    registerSubView(nameEditText_);
    registerSubView(infoText_);
    registerSubView(confirmButton_);
    registerSubView(cancelButton_);

    auto keyFilter = [&](int key) { return processKey(key); };
    nameEditText_->setHeight(3);
    nameEditText_->setKeyFilter(keyFilter);
    nameEditText_->setBorder(curses::DefaultBorder());
    nameEditText_->setOnTextChangedListener(tagNameChangedListener_);
    infoText_->setText("Tag name already exists.");
    infoText_->resizeToText();

    confirmButton_->setText("  Confirm  ");
    confirmButton_->resizeToText();
    confirmButton_->setInFocusBorder(SelectionBorder());
    confirmButton_->setKeyFilter(keyFilter);

    cancelButton_->setText("  Cancel   ");
    cancelButton_->resizeToText();
    cancelButton_->setInFocusBorder(SelectionBorder());
    cancelButton_->setOnButtonClicked([&](int/* key*/) { BaseView::looseFocus(); });
    cancelButton_->setKeyFilter(keyFilter);

    std::vector<ViewFocusOperator::Focusable> focusables {
        { nameEditText_,  [ ]() { return true; } },
        { confirmButton_, [&]() { return true; } },
        { cancelButton_,  [&]() { return true; } },
    };
    focusOperator_->setFocusables(focusables);
    initializeKeyMap();
}

TagCreateView::~TagCreateView() = default;

void TagCreateView::readInput()
{
    focus();
    focusOperator_->resetFocus();
    while(hasFocus() && focusOperator_->isFocusing())
    {
        if (auto view = focusOperator_->viewInFocus())
        {
            view->readInput();
        }
        else
        {
            looseFocus();
        }
    }
}

void TagCreateView::setInfoText(const std::string& text)
{
    infoText_->setText(text);
    infoText_->resizeToText();
    infoText_->setWidth(std::min(infoText_->getWidth(), getWidth() - 2));
    infoText_->setHeight(std::min(infoText_->getHeight(), 3));
    refreshOnRequest();
}

void TagCreateView::setInfoTextVisible(bool visible)
{
    infoText_->setVisible(visible);
    refreshOnRequest();
}

void TagCreateView::displayContent()
{
    nameEditText_->setWidth(getWidth() - 2);
    nameEditText_->setY(1);
    tools::CenterComponent(*nameEditText_, HORIZONTAL, *this);

    infoText_->setY(nameEditText_->getY() + nameEditText_->getHeight());
    infoText_->setX(getWidth() - infoText_->getWidth() - 1);

    cancelButton_->setY(getHeight() - cancelButton_->getHeight() - 1);
    cancelButton_->setX(getWidth() - cancelButton_->getWidth() - 1);
    confirmButton_->setY(cancelButton_->getY());
    confirmButton_->setX(cancelButton_->getX() - confirmButton_->getWidth());
}

bool TagCreateView::processKey(int key)
{
    // TODO: duplicated code (see TagPickerView::processKey()).
    auto keyIter = keyMap_.find(key);
    const bool keyRecognized = (keyIter != std::end(keyMap_));
    if (keyRecognized)
    {
        auto& keyFunc = keyIter->second;
        keyFunc();
        return true;
    }
    return false;
}

void TagCreateView::setOnConfirmButtonClicked(const std::function<void(int)>& action)
{
    confirmButton_->setOnButtonClicked(action);
}

void TagCreateView::setOnCancelButtonClicked(const std::function<void(int)>& action)
{
    cancelButton_->setOnButtonClicked(action);
}

void TagCreateView::setOnTagNameChanged(const std::function<void(const std::string&)>& action)
{
    tagNameChangedListener_->setCallback(action);
}

void TagCreateView::setTagName(const std::string& name)
{
    nameEditText_->setText(name);
}

const std::string& TagCreateView::tagName() const
{
    return nameEditText_->text();
}

void TagCreateView::setColour(const Colour&/* colour*/)
{
    // TODO: not implemented
}

Colour TagCreateView::colour() const
{
    return { 0, 0, 0 }; // TODO: not implemented
}

void TagCreateView::initializeKeyMap()
{
    keyMap_[curses::Key::kEsc] = [&]() {
        focusOperator_->stopFocus();
    };

    keyMap_[curses::Key::kTab] = [&]() {
        focusOperator_->selectNext();
    };

    keyMap_[curses::Key::kShiftTab] = [&]() {
        focusOperator_->selectPrev();
    };
}

void TagCreateView::onKeyFilterSet(const std::function<bool(int)>&/* filterFunction*/)
{
    // filterKey(key) already calls the filterFunction.
    auto filter = [&](int key) { return filterKey(key) || processKey(key); };
    nameEditText_->setKeyFilter(filter);
    confirmButton_->setKeyFilter(filter);
    cancelButton_->setKeyFilter(filter);
}

void TagNameChangedListener::onTextChanged(const std::string& text)
{
    if (callback_)
        callback_(text);
}

void TagNameChangedListener::setCallback(const std::function<void(const std::string&)>& callback)
{
    callback_ = callback;
}

namespace {

Border SelectionBorder()
{
    auto border = curses::DefaultBorder();
    border.bottom = border.top = '-';
    border.left = border.right = ':';
    return border;
}

}
}