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
    explicit Text(const std::string& iText);
    Text(const std::string& iText, const Position& iPosition);

    ~Text();

    void setText(const std::string& iText);
    const std::string& getText() const;

    void setHeight(int iHeight) override {};
    void setWidth(int iWidth) override {};
    void setSize(const Size& iSize) override {};

    int length();
    bool empty();

private:
    std::string text_;
};

} // namespace widget
} // namespace ui
} // namespace memo
