#pragma once
#include "view/BaseView.hpp"
#include "utils/Enums.hpp"

#include <string>

namespace memo::ui {

class TextView : public ui::BaseView
{
public:
    explicit TextView(IComponent* parent=nullptr);
    explicit TextView(const Size& size, IComponent* parent=nullptr);
    TextView(const Size& size, const Position& position, IComponent* parent=nullptr);

    ~TextView() override;

    void setText(const std::string& text);
    const std::string& text() const;

    virtual void setTextAlignment(Align alignment);
    Align textAlignment() const;

    size_t textLength() const;
    bool empty() const;

    void resizeToText();

protected:
    void printForeground() override;
    Rect computeAvailableTextArea();

private:
    std::string text_;
    Align textAlignment_ = Align::TOP_LEFT;
};

} // namespace memo::ui
