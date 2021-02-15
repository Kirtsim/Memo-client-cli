#include "view/widget/TextView.hpp"

namespace memo {
namespace ui {

namespace {
    Size TextSize(const std::string& text);
} // namespace

TextView::TextView(IComponent* parent)
    : TextView(Size(), Position(), parent)
{
}

TextView::TextView(const Size& size, IComponent* parent)
    : TextView(size, Position(), parent)
{
}

TextView::TextView(const Size& size, const Position& position, IComponent* parent)
    : BaseView(size, position, parent)
{
}

TextView::~TextView() = default;

void TextView::setText(const std::string& text)
{
    text_ = text;
    //setSize(TextSize(text_));
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
    size_t lineCount = 0, maxLineLen = 0;
    size_t lastPos = 0;
    size_t pos = text.find('\n');
    while(pos != std::string::npos)
    {
        maxLineLen = std::max(maxLineLen, pos - lastPos);
        lastPos = pos;
        lineCount++;
        pos = text.find('\n', lastPos + 1);
    }
    maxLineLen = std::max(maxLineLen, text.size() - lastPos);

    Size size;
    size.width = maxLineLen;
    size.height = lineCount;
    return size;
}

} // namespace

} // namespace ui
} // namespace memo
