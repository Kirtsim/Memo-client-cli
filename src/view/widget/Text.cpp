#include "view/widget/Text.hpp"

namespace memo {
namespace ui {
namespace widget {

namespace {
    Size TextSize(const std::string& iText);
} // namespace

Text::Text() :
    Text("", Position())
{}

Text::Text(const std::string& iText) :
    Text(iText, Position())
{}

Text::Text(const std::string& iText, const Position& iPosition) :
    ui::BaseComponent(TextSize(iText), iPosition),
    text_(iText)
{}

Text::~Text() = default;

void Text::setText(const std::string& iText)
{
    text_ = iText;
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

Size TextSize(const std::string& iText)
{
    int lineCount = 0; int maxLineLen = 0;
    int lastPos = 0;
    int pos = iText.find('\n');
    while(pos != std::string::npos)
    {
        maxLineLen = std::max(maxLineLen, pos - lastPos);
        lastPos = pos;
        lineCount++;
        pos = iText.find('\n', lastPos + 1);
    }
    maxLineLen = std::max(maxLineLen, static_cast<int>(iText.size()) - lastPos);

    Size size;
    size.width = maxLineLen;
    size.height = lineCount;
    return size;
}

} // namespace

} // namespace widget
} // namespace ui
} // namespace memo
