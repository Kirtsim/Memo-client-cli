#pragma once
#include "view/widget/TextView.hpp"

namespace memo::ui {

class OnTextChangedListener;

class TextEditView : public TextView
{
public:
    explicit TextEditView(IComponent* parent=nullptr);
    explicit TextEditView(const Size& size, IComponent* parent=nullptr);
    TextEditView(const Size& size, const Position& position, IComponent* parent=nullptr);

    ~TextEditView() override;

    void setTextAlignment(Align alignment) override;

    void readInput() override;

    void setOnTextChangedListener(const std::shared_ptr<OnTextChangedListener>& listener);

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
    std::shared_ptr<OnTextChangedListener> textChangedListener_;
};

class OnTextChangedListener
{
public:
    virtual ~OnTextChangedListener() = default;

    virtual void onTextChanged(const std::string& text) = 0;
};
} // namespace memo::ui
