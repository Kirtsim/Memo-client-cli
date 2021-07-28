#include "view/dialog/ConfirmDialog.hpp"
#include "view/widget/TextView.hpp"
#include "ncurses/functions.hpp"
#include "ncurses/keys.hpp"
#include "view/tools/Tools.hpp"

namespace memo::ui {

namespace {

    memo::Border buttonSelectedBorder();

} // namespace

bool ConfirmDialog::Display(const std::string& message, IComponent* parent)
{
    ConfirmDialog dialog(parent);
    dialog.setMessage(message);
    if (parent)
        tools::Tools::centerComponent(dialog, Center::CENTER, *parent);
    return dialog.display();
}

ConfirmDialog::ConfirmDialog(IComponent* parent)
    : ConfirmDialog(Size(), Position(), parent)
{
}

ConfirmDialog::ConfirmDialog(const Size& size, IComponent* parent)
    : ConfirmDialog(size, Position(), parent)
{
}

ConfirmDialog::ConfirmDialog(const Size& size, const Position& position, IComponent* parent)
    : BaseView(size, position, parent)
    , message_(std::make_shared<TextView>(this))
    , okButton_(std::make_shared<TextView>(this))
    , cancelButton_(std::make_shared<TextView>(this))
{
    auto border = curses::DefaultBorder();
    border.top = border.bottom = '=';
    border.left = border.right = '+';
    BaseView::setBorder(border);
    registerSubView(message_);
    registerSubView(okButton_);
    registerSubView(cancelButton_);

    okButton_->setText("   OK   ");
    okButton_->setBorder(buttonSelectedBorder());
    okButton_->resizeToText();

    cancelButton_->setText(" Cancel ");
    cancelButton_->setBorder(curses::DefaultBorder());
    cancelButton_->resizeToText();
}

ConfirmDialog::~ConfirmDialog() = default;

void ConfirmDialog::setMessage(const std::string& message)
{
    message_->setText(message);
}

const std::string& ConfirmDialog::message() const
{
    return message_->text();
}

bool ConfirmDialog::display()
{
    refresh();
    curses::KeyPad(this->getWindow(), ENABLE);
    const auto prevCursorState = curses::CursorVisible(false);
    bool confirmed = false;
    while (true)
    {
        int key = curses::ReadChar(this->getWindow());
        if (key == curses::Key::kEsc)
            break;
        else if (key == curses::Key::kEnter)
        {
            confirmed = okButtonSelected_;
            break;
        }
        else if (key == curses::Key::kTab || key == curses::Key::kShiftTab)
        {
            if (okButtonSelected_)
            {
                okButton_->setBorder(curses::DefaultBorder());
                cancelButton_->setBorder(buttonSelectedBorder());
            }
            else
            {
                okButton_->setBorder(buttonSelectedBorder());
                cancelButton_->setBorder(curses::DefaultBorder());
            }
            okButtonSelected_ = !okButtonSelected_;
            refreshButtons();
        }
    }
    curses::CursorVisible(prevCursorState);
    curses::KeyPad(this->getWindow(), DISABLE);
    parentRequestOnRefresh();
    return confirmed;
}

void ConfirmDialog::refreshButtons()
{
    okButton_->refreshOnRequest();
    cancelButton_->refreshOnRequest();
    okButton_->refresh();
    cancelButton_->refresh();
}

void ConfirmDialog::beforeViewResized()
{   // TODO: this is duplicated code from MessageDialog
    BaseView::beforeViewResized();
    message_->resizeToText();
    if (message_->getWidth() > kMaxWidth)
        message_->setWidth(kMaxWidth);

    updateComponentsPosition();
    const int height = okButton_->getY() + okButton_->getHeight() + kPaddingVertical;
    const int width = kPaddingHorizontal + message_->getWidth() + kPaddingHorizontal;

    setSize({ Height(height), Width(width) });
    if (auto parent = getParent())
        tools::Tools::centerComponent(*this, Center::CENTER, *parent);
    updateComponentsPosition();
}

void ConfirmDialog::updateComponentsPosition()
{
    message_->setX(kPaddingHorizontal);
    message_->setY(kPaddingVertical);

    okButton_->setY(message_->getY() + message_->getHeight() + kPaddingVertical);
    cancelButton_->setY(okButton_->getY());

    const int buttonSpacing = 3;
    const int totalBtnWidth = okButton_->getWidth() + buttonSpacing + cancelButton_->getWidth();
    const int okButtonX = (getWidth() / 2) - (totalBtnWidth / 2);
    okButton_->setX(okButtonX);
    cancelButton_->setX(okButtonX + okButton_->getWidth() + buttonSpacing);
}

namespace {

memo::Border buttonSelectedBorder()
{
    auto border = memo::curses::DefaultBorder();
    border.top = border.bottom = '=';
    border.left = border.right = ':';

    return border;
}

} // namespace
} // namespace memo::ui

