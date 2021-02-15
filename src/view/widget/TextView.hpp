#pragma once
#include "view/BaseView.hpp"

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

    int length();
    bool empty();

private:
    std::string text_;
};

} // namespace ui
} // namespace memo
