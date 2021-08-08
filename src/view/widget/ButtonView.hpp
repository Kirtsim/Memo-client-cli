#pragma once
#include "view/BaseView.hpp"
#include "utils/Enums.hpp"

namespace memo::ui {
class TextView;

class ButtonView : public ui::BaseView
{
public:
    explicit ButtonView(IComponent* parent=nullptr);
    explicit ButtonView(const Size& size, IComponent* parent=nullptr);
    ButtonView(const Size& size, const Position& position, IComponent* parent=nullptr);

    ~ButtonView() override;

    void setText(const std::string& text);
    const std::string& text() const;

    virtual void setTextAlignment(Align alignment);
    Align textAlignment() const;

    void setBorder(const Border& border) override;

    Border getBorder() const override;

    void setInFocusBorder(const Border& border);

    const Border& inFocusBorder();

    void resizeToText();

protected:
    void onSizeChanged(const Size& oldSize, const Size& newSize) override;

    void displayContent() override;


private:
    std::shared_ptr<TextView> text_;

    Border inFocusBorder_;
    Border outOfFocusBorder_;
};

} // namespace memo::ui