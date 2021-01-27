#include "view/widget/TextView.hpp"

namespace memo {
namespace ui {

namespace {
    Size TextSize(const std::string& text);
} // namespace

TextView::TextView() :
    TextView("", Position())
{}

TextView::TextView(const std::string& text) :
    TextView(text, Position())
{}

TextView::TextView(const std::string& text, const Position& position) :
    BaseView(TextSize(text), position),
    text_(text)
{}

TextView::~TextView() = default;

void TextView::setText(const std::string& text)
{
    text_ = text;
    setSize(TextSize(text_));
}

const std::string& TextView::text() const
{
    return text_;
}

int TextView::length()
{
    return text_.size();
}

bool TextView::empty()
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
