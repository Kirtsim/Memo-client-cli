#include "view/widget/TextView.hpp"
#include "view/tools/StringTools.hpp"
#include "ncurses/functions.hpp"
#include "ncurses/Window.hpp"

namespace memo::ui {

namespace {
    bool hasTopBorder(const Border& border);
    bool hasBotBorder(const Border& border);
    bool hasLeftBorder(const Border& border);
    bool hasRightBorder(const Border& border);
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
    BaseView::setBorder(Border::NoBorder());
}

TextView::~TextView() = default;

void TextView::setText(const std::string& text)
{
    if (text_ != text)
    {
        refreshOnRequest();
        text_ = text;
    }
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

size_t TextView::textLength() const
{
    return text_.size();
}

bool TextView::empty() const
{
    return text_.empty();
}

void TextView::printForeground()
{
    auto textArea = computeAvailableTextArea();
    auto lines = tools::splitIntoLines(text_, static_cast<size_t>(textArea.width));
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

        const auto line = lines[lineIdx].substr(0, static_cast<size_t>(textArea.width));
        const int lineWidth = static_cast<int>(line.size());
        if (textAlignment_ & Align::CENTER_HORIZONTAL)
            textPos.x = textArea.x + (textArea.width / 2) - (lineWidth / 2);
        else if (textAlignment_ & Align::RIGHT)
            textPos.x = (textArea.x + textArea.width) - lineWidth;
        textPos.x = std::max(textPos.x, textArea.x);

        curses::PrintText(std::string(static_cast<size_t>(textArea.width), ' '), getWindow(), textPos);
        curses::PrintText(line, getWindow(), textPos);

        textPos.y++;
        ++lineIdx;
        --linesLeft;
    }
    // TODO: Clear any leftover lines from a longer string that was printed before (this can happen if the TextView
    //  is not resized to fit the text after the text has changed.
}

void TextView::resizeToText()
{
    const auto textArea = computeAvailableTextArea().width;
    auto lines = tools::splitIntoLines(text_, static_cast<size_t>(textArea));
    Size textSize;
    textSize.height = static_cast<int>(lines.size());

    for (const auto& line : lines)
    {
        const int lineWidth = static_cast<int>(line.size());
        textSize.width = std::max(lineWidth, textSize.width);
    }

    const auto border = getBorder();
    if (hasTopBorder(border))
        textSize.height += 1;
    if (hasBotBorder(border))
        textSize.height += 1;
    if (hasLeftBorder(border))
        textSize.width += 1;
    if (hasRightBorder(border))
        textSize.width += 1;
    setSize(textSize);
    refreshOnRequest();
}

Rect TextView::computeAvailableTextArea()
{
    const auto border = getBorder();
    auto area = Rect(getSize());

    if (hasLeftBorder(border))
    {
        area.width--;
        area.x = 1;
    }
    if (hasRightBorder(border))
    {
        area.width--;
    }
    if (hasTopBorder(border))
    {
        area.height--;
        area.y = 1;
    }
    if (hasBotBorder(border))
    {
        area.height--;
    }
    return area;
}

namespace {
bool hasTopBorder(const Border& border)
{
    return border.top != ' ' || border.corner.upperLeft != ' ' || border.corner.upperRight != ' ';
}

bool hasBotBorder(const Border& border)
{
    return border.bottom != ' ' || border.corner.lowerLeft != ' ' || border.corner.lowerRight != ' ';
}

bool hasLeftBorder(const Border& border)
{
    return border.left != ' ' || border.corner.upperLeft != ' ' || border.corner.lowerLeft != ' ';
}

bool hasRightBorder(const Border& border)
{
    return border.right != ' ' || border.corner.upperRight != ' ' || border.corner.lowerRight != ' ';
}

} // namespace

} // namespace memo
