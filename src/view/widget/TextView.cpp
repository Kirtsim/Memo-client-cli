#include "view/widget/TextView.hpp"
#include "ncurses/functions.hpp"

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
}

const std::string& TextView::text() const
{
    return text_;
}

void TextView::setTextAlignment(Align alignment)
{
    textAlignment_ = alignment;
}

Align TextView::textAlignment() const
{
    return textAlignment_;
}

int TextView::length()
{
    return text_.size();
}

bool TextView::empty()
{
    return text_.empty();
}

void TextView::displayContent()
{
    auto textSize = TextSize(text_);
    Position textPos;

    if (textAlignment_ & Align::CENTER_HORIZONTAL)
        textPos.x += (getWidth() / 2) - (textSize.width / 2);
    else if (textAlignment_ & Align::RIGHT)
        textPos.x += getWidth() - textSize.width - 1;
    else
        textPos.x += 1;

    if (textAlignment_ & Align::CENTER_VERTICAL)
        textPos.y += (getHeight() / 2) - (textSize.height / 2);
    else if (textAlignment_ & Align::BOTTOM)
        textPos.y += getHeight() - textSize.height - 1;
    else
        textPos.y += 1;

    curses::PrintText(text_, getWindow(), textPos);
}

namespace {

Size TextSize(const std::string& text)
{
    if (text.empty())
        return {};
    size_t lineCount = 1, maxLineLen = 0;
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
