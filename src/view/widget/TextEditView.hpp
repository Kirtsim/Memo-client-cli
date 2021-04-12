#pragma once
#include "view/widget/TextView.hpp"

namespace memo {
namespace ui {

class KeyFilter;

class TextEditView : public TextView
{
public:
    explicit TextEditView(IComponent* parent=nullptr);
    explicit TextEditView(const Size& size, IComponent* parent=nullptr);
    TextEditView(const Size& size, const Position& position, IComponent* parent=nullptr);

    ~TextEditView();

    void setTextAlignment(Align alignment) override;

    void readInput();
    void setKeyFilter(const std::shared_ptr<KeyFilter>& filter);

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
    std::shared_ptr<KeyFilter> keyFilter_ = nullptr;
};

class KeyFilter
{
public:
    /// Filters the input key. Returns true if the key was processed. The key
    /// is then ignored by the View. If false is returned, the View will
    /// process the key.
    virtual bool filterKey(int key) = 0;
};

} // namespace ui
} // namespace memo
