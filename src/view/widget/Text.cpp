#include "view/widget/Text.hpp"

namespace memo {
namespace ui {

namespace {
    Size TextSize(const std::string& text);
} // namespace

Text::Text() :
    Text("", Position())
{}

Text::Text(const std::string& text) :
    Text(text, Position())
{}

Text::Text(const std::string& text, const Position& position) :
    ui::BaseComponent(TextSize(text), position),
    text_(text)
{}

Text::~Text() = default;

void Text::setText(const std::string& text)
{
    text_ = text;
    setSize(TextSize(text_));
}

const std::string& Text::getText() const
{
    return text_;
}

int Text::length()
{
    return text_.size();
}

bool Text::empty()
{
    return text_.empty();
}

namespace {

Size TextSize(const std::string& text)
{
    int lineCount = 0; int maxLineLen = 0;
    int lastPos = 0;
    int pos = text.find('\n');
    while(pos != std::string::npos)
    {
        maxLineLen = std::max(maxLineLen, pos - lastPos);
        lastPos = pos;
        lineCount++;
        pos = text.find('\n', lastPos + 1);
    }
    maxLineLen = std::max(maxLineLen, static_cast<int>(text.size()) - lastPos);

    Size size;
    size.width = maxLineLen;
    size.height = lineCount;
    return size;
}

} // namespace

} // namespace ui
} // namespace memo
