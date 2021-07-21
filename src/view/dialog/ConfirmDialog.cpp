#include "view/dialog/ConfirmDialog.hpp"
#include "view/tools/Tools.hpp"
#include "ncurses/functions.hpp"
#include "ncurses/keys.hpp"

namespace memo::ui {

void ConfirmDialog::Display(const std::string& message, IComponent* parent)
{
    ConfirmDialog dialog(parent);
    dialog.setMessage(message);
    if (parent)
        tools::Tools::centerComponent(dialog, Center::CENTER, *parent);
    dialog.display();
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
    , message_(std::make_shared<TextView>())
    , okButton_(std::make_shared<TextView>())
{
    auto border = curses::DefaultBorder();
    border.top = border.bottom = '=';
    border.left = border.right = '+';
    BaseView::setBorder(border);
    registerSubView(message_);
    registerSubView(okButton_);
    okButton_->setText("< OK >");
    okButton_->setBorder(curses::DefaultBorder());
    okButton_->resizeToText();
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

void ConfirmDialog::display()
{
    refresh();
    curses::KeyPad(this->getWindow(), ENABLE);
    bool confirmed = false;
    while (!confirmed)
    {
        int key = curses::ReadChar(this->getWindow());
        confirmed = (key == curses::Key::kEnter || key == curses::Key::kEsc);
    }
    curses::KeyPad(this->getWindow(), DISABLE);
}

void ConfirmDialog::beforeViewResized()
{
    BaseView::beforeViewResized();
    message_->resizeToText();
    if (message_->getWidth() > kMaxWidth)
        message_->setWidth(kMaxWidth);

    updateComponentsPosition();
    const int height = okButton_->getY() + okButton_->getHeight() + kPaddingVertical;
    const int width = kPaddingHorizontal + message_->getWidth() + kPaddingHorizontal;

    setSize({ Height(height), Width(width) });
    if (auto parent = getParent())
        tools::Tools::centerComponent(*this, Center::CENTER, *parent); // TODO: use some center policy (a flag or sth). The user might not want it centered.
    updateComponentsPosition();
}

void ConfirmDialog::positionComponents()
{
    BaseView::positionComponents();
}

void ConfirmDialog::onPositionChanged(const Position& oldPos, const Position& newPos)
{
    BaseView::onPositionChanged(oldPos, newPos);
}

void ConfirmDialog::updateComponentsPosition()
{
    message_->setX(kPaddingHorizontal);
    message_->setY(kPaddingVertical);

    okButton_->setX(kPaddingHorizontal);
    okButton_->setY(message_->getY() + message_->getHeight() + kPaddingVertical);
    tools::Tools::centerComponent(*okButton_, Center::HORIZONTAL, *this);
}

} // namespace memo::ui
