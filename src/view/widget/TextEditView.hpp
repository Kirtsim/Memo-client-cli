#pragma once
#include "view/widget/TextView.hpp"

namespace memo {
namespace ui {

class TextEditView : public TextView
{
public:
    explicit TextEditView(IComponent* parent=nullptr);
    explicit TextEditView(const Size& size, IComponent* parent=nullptr);
    TextEditView(const Size& size, const Position& position, IComponent* parent=nullptr);

    ~TextEditView();

    void focus() override;
    void setTextAlignment(Align alignment) override;

private:
    void processInputCharacter(int character);
    void printCharacter(int character);

    void moveCursorLeft();
    void moveCursorRight();
    void moveCursorUp();
    void moveCursorDown();
    void applyBackSpace();

private:
    Position cursorPosition_ = { PosX(1), PosY(1) };
};

} // namespace ui
} // namespace memo
