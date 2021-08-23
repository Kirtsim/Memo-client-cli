#include "view/widget/ButtonView.hpp"
#include "view/widget/TextView.hpp"
#include "view/tools/Tools.hpp"
#include "ncurses/functions.hpp"
#include "ncurses/IWindow.hpp"
#include "ncurses/keys.hpp"

namespace memo::ui {

ButtonView::ButtonView(IComponent* parent)
    : ButtonView(Size(), Position(), parent)
{
}

ButtonView::ButtonView(const Size& size, IComponent* parent)
    : ButtonView(size, Position(), parent)
{
}

ButtonView::ButtonView(const Size& size, const Position& position, IComponent* parent)
    : BaseView(size, position, parent)
    , text_(std::make_shared<TextView>())
    , inFocusBorder_(curses::DefaultBorder())
    , outOfFocusBorder_(curses::DefaultBorder())
{
    registerSubView(text_);
    text_->setBorder(outOfFocusBorder_);
    buttonClickKeys_.insert(curses::Key::kEnter);
}

ButtonView::~ButtonView() = default;

void ButtonView::setText(const std::string& text)
{
    text_->setText(text);
    refreshOnRequest();
}

const std::string& ButtonView::text() const
{
    return text_->text();
}

void ButtonView::setTextAlignment(Align alignment)
{
    text_->setTextAlignment(alignment);
    refreshOnRequest();
}

Align ButtonView::textAlignment() const
{
    return text_->textAlignment();
}

void ButtonView::resizeToText()
{
    text_->resizeToText();
    setSize(text_->getSize());
    refreshOnRequest();
}

void ButtonView::setOnButtonClicked(const std::function<void(int)>& perform)
{
    onButtonClicked_ = perform;
}

void ButtonView::listenToKeys(const std::vector<int>& keys)
{
    buttonClickKeys_.clear();
    buttonClickKeys_.insert(keys.begin(), keys.end());
}

void ButtonView::readInput()
{
    focus();
    curses::KeyPad(getWindow(), ENABLE);
    const auto wasCursorVisible = curses::CursorVisible(false);

    tools::ForceRefresh(this);
    while (hasFocus() && !buttonClickKeys_.empty())
    {
        const int key = curses::ReadChar(getWindow());
        if (filterKey(key))
            continue;

        const bool buttonClicked = (buttonClickKeys_.find(key) != std::end(buttonClickKeys_));
        if (buttonClicked)
        {
            if (onButtonClicked_)
                onButtonClicked_(key);
            looseFocus();
        }
    }


    tools::ForceRefresh(this);
    curses::CursorVisible(wasCursorVisible);
    curses::KeyPad(getWindow(), DISABLE);
}

void ButtonView::displayContent()
{
    text_->setPosition({ PosX(0), PosY(0)});
    text_->setBorder(hasFocus() ? inFocusBorder_ : outOfFocusBorder_);
}

void ButtonView::onSizeChanged(const Size& oldSize, const Size& newSize)
{
    BaseView::onSizeChanged(oldSize, newSize);
    text_->setSize(newSize);
}

void ButtonView::setBorder(const Border& border)
{
    outOfFocusBorder_ = border;
    refreshOnRequest();
}

Border ButtonView::getBorder() const
{
    return text_->getBorder();
}

void ButtonView::setInFocusBorder(const Border& border)
{
    inFocusBorder_ = border;
    refreshOnRequest();
}

const Border& ButtonView::inFocusBorder()
{
    return inFocusBorder_;
}

}
