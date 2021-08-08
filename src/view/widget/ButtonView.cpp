#include "view/widget/ButtonView.hpp"
#include "view/widget/TextView.hpp"
#include "view/tools/Tools.hpp"
#include "ncurses/functions.hpp"
#include "ncurses/IWindow.hpp"

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
