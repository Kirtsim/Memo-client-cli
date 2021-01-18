#pragma once
#include "view/BaseComponent.hpp"

#include <string>

namespace memo {
namespace ui {
namespace widget {

class Text : public ui::BaseComponent
{
public:
    Text();
    explicit Text(const std::string& text);
    Text(const std::string& text, const Position& position);

    ~Text();

    void setText(const std::string& text);
    const std::string& getText() const;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
    void setHeight(int height) override {};
    void setWidth(int width) override {};
    void setSize(const Size& size) override {};
#pragma GCC diagnostic pop

    int length();
    bool empty();

private:
    std::string text_;
};

} // namespace widget
} // namespace ui
} // namespace memo
