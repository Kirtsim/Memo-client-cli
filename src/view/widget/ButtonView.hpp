#pragma once
#include "view/BaseView.hpp"
#include "utils/Enums.hpp"

#include <functional>
#include <set>
#include <vector>

namespace memo::ui {
class TextView;

class ButtonView : public ui::BaseView
{
public:
    explicit ButtonView(IComponent* parent=nullptr);
    explicit ButtonView(const Size& size, IComponent* parent=nullptr);
    ButtonView(const Size& size, const Position& position, IComponent* parent=nullptr);

    ~ButtonView() override;

    ButtonView(const ButtonView&) = delete;
    ButtonView(const ButtonView&&) = delete;
    ButtonView& operator=(const ButtonView&) = delete;

    void setText(const std::string& text);
    const std::string& text() const;

    virtual void setTextAlignment(Align alignment);
    Align textAlignment() const;

    void setBorder(const Border& border) override;

    Border getBorder() const override;

    void setInFocusBorder(const Border& border);

    const Border& inFocusBorder();

    void resizeToText();

    void setOnButtonClicked(const std::function<void(int)>& perform);

    void listenToKeys(const std::vector<int>& keys);

    void readInput() override;

protected:
    void onSizeChanged(const Size& oldSize, const Size& newSize) override;

    void displayContent() override;


private:
    std::shared_ptr<TextView> text_;
    std::function<void(int)> onButtonClicked_;

    std::set<int> buttonClickKeys_;

    Border inFocusBorder_;
    Border outOfFocusBorder_;
};

} // namespace memo::ui