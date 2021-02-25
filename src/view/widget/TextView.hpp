#pragma once
#include "view/BaseView.hpp"
#include "utils/Enums.hpp"

#include <string>

namespace memo {
namespace ui {

class TextView : public ui::BaseView
{
public:
    explicit TextView(IComponent* parent=nullptr);
    explicit TextView(const Size& size, IComponent* parent=nullptr);
    TextView(const Size& size, const Position& position, IComponent* parent=nullptr);

    ~TextView();

    void setText(const std::string& text);
    const std::string& text() const;

    void setTextAlignment(Align alignment);
    Align textAlignment() const;

    int length();
    bool empty();

    void resizeToText();

protected:
    void displayContent() override;
    void printForeground() override;

private:
    Rect computeAvailableTextArea();

private:
    std::string text_;
    Align textAlignment_ = Align::TOP_LEFT;
};

} // namespace ui
} // namespace memo
