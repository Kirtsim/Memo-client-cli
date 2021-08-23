#include "view/dialog/MessageDialog.hpp"
#include "view/tools/Tools.hpp"
#include "ncurses/functions.hpp"
#include "ncurses/keys.hpp"

namespace memo::ui {

void MessageDialog::Display(const std::string& message, IComponent* parent)
{
    MessageDialog dialog(parent);
    dialog.setMessage(message);
    if (parent)
        tools::CenterComponent(dialog, Center::CENTER, *parent);
    dialog.display();
}

MessageDialog::MessageDialog(IComponent* parent)
    : MessageDialog(Size(), Position(), parent)
{
}

MessageDialog::MessageDialog(const Size& size, IComponent* parent)
    : MessageDialog(size, Position(), parent)
{
}

MessageDialog::MessageDialog(const Size& size, const Position& position, IComponent* parent)
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

MessageDialog::~MessageDialog() = default;

void MessageDialog::setMessage(const std::string& message)
{
    message_->setText(message);
}

const std::string& MessageDialog::message() const
{
    return message_->text();
}

void MessageDialog::display()
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

void MessageDialog::beforeViewResized()
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
        tools::CenterComponent(*this, Center::CENTER, *parent); // TODO: use some center policy (a flag or sth). The user might not want it centered.
    updateComponentsPosition();
}

void MessageDialog::positionComponents()
{
    BaseView::positionComponents();
}

void MessageDialog::onPositionChanged(const Position& oldPos, const Position& newPos)
{
    BaseView::onPositionChanged(oldPos, newPos);
}

void MessageDialog::updateComponentsPosition()
{
    message_->setX(kPaddingHorizontal);
    message_->setY(kPaddingVertical);

    okButton_->setX(kPaddingHorizontal);
    okButton_->setY(message_->getY() + message_->getHeight() + kPaddingVertical);
    tools::CenterComponent(*okButton_, Center::HORIZONTAL, *this);
}

} // namespace memo::ui
