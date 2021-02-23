#include "view/widget/TextView.hpp"
#include "ncurses/functions.hpp"
#include "ncurses/Window.hpp"

#include <vector>

namespace memo {
namespace ui {

namespace {
    std::vector<std::string> SplitIntoLines(const std::string& text);
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
    curses::PrintText("This is a TextView", getWindow(), { PosX(1), PosY(0) });
}

void TextView::printForeground()
{
    auto textArea = computeAvailableTextArea();
    auto lines = SplitIntoLines(text_);
    auto textPos = textArea.position();

    int textHeight = static_cast<int>(lines.size());

    if (textAlignment_ & Align::CENTER_VERTICAL)
        textPos.y = textArea.y + (textArea.height / 2) - (textHeight / 2);
    else if (textAlignment_ & Align::BOTTOM)
        textPos.y = (textArea.y + textArea.height) - textHeight;
    textPos.y = std::max(textPos.y, textArea.y);

    int linesLeft = std::min(textHeight, textArea.height);
    size_t lineIdx = 0;
    while (linesLeft > 0)
    {
        textPos.x = textArea.x;

        const auto line = lines[lineIdx].substr(0, textArea.width);
        const int lineWidth = static_cast<int>(line.size());
        if (textAlignment_ & Align::CENTER_HORIZONTAL)
            textPos.x = textArea.x + (textArea.width / 2) - (lineWidth / 2);
        else if (textAlignment_ & Align::RIGHT)
            textPos.x = (textArea.x + textArea.width) - lineWidth;
        textPos.x = std::max(textPos.x, textArea.x);

        curses::PrintText(line, getWindow(), textPos);

        textPos.y++;
        ++lineIdx;
        --linesLeft;
    }
    auto pos = Position();
}

Rect TextView::computeAvailableTextArea()
{
    const auto border = getBorder();
    auto area = Rect(getSize());

    bool hasTopBorder = border.top    != ' ' || border.corner.upperLeft != ' ' || border.corner.upperRight != ' ';
    bool hasBotBorder = border.bottom != ' ' || border.corner.lowerLeft != ' ' || border.corner.lowerRight != ' ';
    bool hasLeftBorder  = border.left  != ' ' || border.corner.upperLeft != ' ' || border.corner.lowerLeft != ' ';
    bool hasRightBorder = border.right != ' ' || border.corner.upperRight != ' ' || border.corner.lowerRight != ' ';
    if (hasLeftBorder)
    {
        area.width--;
        area.x = 1;
    }
    if (hasRightBorder)
    {
        area.width--;
    }
    if (hasTopBorder)
    {
        area.height--;
        area.y = 1;
    }
    if (hasBotBorder)
    {
        area.height--;
    }
    return area;
}

namespace {

std::vector<std::string> SplitIntoLines(const std::string& text)
{
    std::vector<std::string> lines;

    size_t pos = 0;
    size_t endPos = text.find('\n');
    while(endPos != std::string::npos)
    {
        const auto length = endPos - pos;
        lines.emplace_back(text.substr(pos, length));
        pos = endPos + 1;
        endPos = text.find('\n', pos);
    }
    lines.emplace_back(text.substr(pos));

    return lines;
}

} // namespace

} // namespace ui
} // namespace memo
