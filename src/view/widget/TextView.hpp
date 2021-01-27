#pragma once
#include "view/BaseView.hpp"

#include <string>

namespace memo {
namespace ui {

class TextView : public ui::BaseView
{
public:
    TextView();
    explicit TextView(const std::string& text);
    TextView(const std::string& text, const Position& position);

    ~TextView();

    void setText(const std::string& text);
    const std::string& text() const;

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

} // namespace ui
} // namespace memo
