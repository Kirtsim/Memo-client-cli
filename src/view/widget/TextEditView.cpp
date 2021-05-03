#include "view/widget/TextEditView.hpp"
#include "view/tools/StringTools.hpp"
#include "ncurses/IWindow.hpp"
#include "ncurses/functions.hpp"
#include "ncurses/keys.hpp"
#include <ncurses.h>
#include <numeric>
#include <vector>
#include <set>

namespace memo {
namespace ui {

namespace {
    const std::unordered_set<int> kSpecialChars {
        curses::Key::kDown,
        curses::Key::kRight,
        curses::Key::kUp,
        curses::Key::kLeft,
        curses::Key::kBackSpace,
        curses::Key::kEnter,
        curses::Key::kEsc,
    };

    bool isCharBanned(int character)
    {
        if (kSpecialChars.find(character) != std::end(kSpecialChars))
            return false;
        if (0 <= character && character <= 31)
            return true;
        if (character >= 127)
            return true;
        return false;
    }

    Position mapCursorToText(const Position& cursorPos, const Rect& textArea);
    size_t mapCursorPositionToTextIndex(const Position& cursorPos, const Rect& textArea, const std::string& text);

    inline int count(const std::vector<std::string>& vec);
    inline int len(const std::string& str);
    inline int to_int(size_t value);
    inline size_t to_size_t(int value);

} // namespace


TextEditView::TextEditView(IComponent* parent)
    : TextEditView(Size(), Position(), parent)
{
}

TextEditView::TextEditView(const Size& size, IComponent* parent)
    : TextEditView(size, Position(), parent)
{
}

TextEditView::TextEditView(const Size& size, const Position& position, IComponent* parent)
    : TextView(size, position, parent)
{
    setBorder(Border::NoBorder());
    TextView::setTextAlignment(Align::TOP_LEFT);
}

TextEditView::~TextEditView() = default;

void TextEditView::readInput()
{
    curses::KeyPad(getWindow(), ENABLE);
    curses::CursorVisible(true);

    while (hasFocus())
    {
        const auto cursorPos = curses::CursorPosition(getWindow());
        auto character = curses::ReadCharAt(getWindow(), cursorPos);
        if (!keyFilter_ || !keyFilter_->filterKey(character))
        {
            processInputCharacter(character);
        }
        getWindow().redraw();
    }

    curses::CursorVisible(false);
    curses::KeyPad(getWindow(), DISABLE);
}

void TextEditView::processInputCharacter(const int character)
{
    if (character == -1 || isCharBanned(character))
        return;
    if (character == curses::Key::kEsc)
        looseFocus(); 
    else if (character == curses::Key::kLeft)
    {
        moveCursorLeft();
    }
    else if (character == curses::Key::kRight)
    {
        moveCursorRight();
    }
    else if (character == curses::Key::kUp)
    {
        moveCursorUp();
    }
    else if (character == curses::Key::kDown)
    {
        moveCursorDown();
    }
    else if (character == curses::Key::kBackSpace)
    {
        applyBackSpace();
    }
    else if (character == curses::Key::kEnter)
    {
        const auto textArea = computeAvailableTextArea();
        const auto lines = tools::splitIntoLines(text(), textArea);
        auto cursPos = curses::CursorPosition(getWindow());

        const auto maxY = textArea.y + textArea.height-1;
        if (count(lines) < textArea.height && cursPos.y < maxY)
        {
            printCharacter(character);
            ++cursPos.y;
            cursPos.x = textArea.x;
            curses::PositionCursor(getWindow(), cursPos);
        }
    }
    else
        printCharacter(character);
}

void TextEditView::printCharacter(int character)
{
    const auto textArea = computeAvailableTextArea();
    auto theText = text();
    auto cursorPos = curses::CursorPosition(getWindow());
    size_t cursTxtIdx = mapCursorPositionToTextIndex(cursorPos, textArea, theText);

    if (cursTxtIdx <= theText.size())
    {
        theText.insert(cursTxtIdx, 1, static_cast<char>(character));
        const auto lines = tools::splitIntoLines(theText, textArea);
        if (count(lines) > textArea.height && lines.back().empty())
            return; 
        if (count(lines) == textArea.height && len(lines.back()) > textArea.width)
        {
            return;
        }
        setText(theText);

        auto pos = textArea.position();
        const auto& window = getWindow();
        const std::string clearLine(static_cast<size_t>(textArea.width), ' ');
        for (const auto& line : lines)
        {
            curses::PrintText(clearLine, window, pos);
            curses::PrintText(line, window, pos);
            ++pos.y;
        }
        ++cursorPos.x;
        if (cursorPos.x > textArea.x + textArea.width - 1)
        {
            ++cursorPos.y;
            cursorPos.x = textArea.x;
            if (cursorPos.y > textArea.y + textArea.height - 1)
            {
                return;
            }
        }
        curses::PositionCursor(window, cursorPos);
    }
}

void TextEditView::moveCursorLeft()
{
    const auto textArea = computeAvailableTextArea();
    auto cursorPos = curses::CursorPosition(getWindow());
    if (cursorPos.x > textArea.x)
        --cursorPos.x;
    else if (cursorPos.y > textArea.y)
    {
        const auto lines = tools::splitIntoLines(text(), textArea);
        const auto cursorTextPos = mapCursorToText(cursorPos, textArea);
        const auto lineIdx = static_cast<size_t>(cursorTextPos.y);
        if (lineIdx > 0)
        {
            --cursorPos.y;
            cursorPos.x = textArea.x + len(lines[lineIdx-1]);
            cursorPos.x = std::min(cursorPos.x, textArea.x + textArea.width - 1);
        }
    }

    curses::PositionCursor(getWindow(), cursorPos);
}

void TextEditView::moveCursorRight()
{
    const auto textArea = computeAvailableTextArea();
    auto cursorPos = curses::CursorPosition(getWindow());
    const auto maxX = textArea.x + textArea.width - 1;
    const auto lines = tools::splitIntoLines(text(), textArea);
    const auto cursorTextPos = mapCursorToText(cursorPos, textArea);
    const auto lineIdx = static_cast<size_t>(cursorTextPos.y);
    if (lineIdx >= lines.size())
        return;
    if (cursorPos.x == maxX || cursorTextPos.x == len(lines[lineIdx]))
    {
        //if (lineIdx + 1 < lines.size() || (lineIdx < lines.size() && lines[lineIdx].size() == static_cast<size_t>(textArea.width)))
        const auto maxY = textArea.y + textArea.height - 1;
        if (cursorPos.y < maxY && lineIdx + 1 < lines.size())
        {
            ++cursorPos.y;
            cursorPos.x = textArea.x;
        }
    }
    else if (cursorTextPos.x < len(lines[lineIdx]))
    {
        ++cursorPos.x;
    }

    curses::PositionCursor(getWindow(), cursorPos);
}

void TextEditView::moveCursorUp()
{
    const auto textArea = computeAvailableTextArea();

    auto cursorPos = curses::CursorPosition(getWindow());
    if (cursorPos.y > textArea.y)
    {
        --cursorPos.y;
        const auto lines = tools::splitIntoLines(text(), textArea);
        if (lines.empty())
            return;
        const auto cursorTextPos = mapCursorToText(cursorPos, textArea);
        const auto lineIdx = static_cast<size_t>(cursorTextPos.y);
        if (lineIdx < lines.size())
        {
            cursorPos.x = std::min(cursorPos.x, textArea.x + len(lines[lineIdx]));
            cursorPos.x = std::min(cursorPos.x, textArea.x + textArea.width - 1);
            curses::PositionCursor(getWindow(), cursorPos);
        }
    }
}

void TextEditView::moveCursorDown()
{
    const auto textArea = computeAvailableTextArea();
    auto cursorPos = curses::CursorPosition(getWindow());

    if (cursorPos.y < textArea.y + textArea.height - 1) // still may want to scroll
    {
        const auto lines = tools::splitIntoLines(text(), textArea);
        if (lines.empty())
            return;

        const auto cursorTextPos = mapCursorToText(cursorPos, textArea);
        const auto lineIdx = static_cast<size_t>(cursorTextPos.y);
        if (lineIdx < lines.size() - 1)
        {
            ++cursorPos.y;
            cursorPos.x = std::min(cursorPos.x, textArea.x + len(lines[lineIdx+1]));
            cursorPos.x = std::min(cursorPos.x, textArea.x + textArea.width - 1);
            curses::PositionCursor(getWindow(), cursorPos);
        }
    }
}

void TextEditView::applyBackSpace()
{
    const auto cursorPos = curses::CursorPosition(getWindow());
    const auto textArea = computeAvailableTextArea();
    if (cursorPos == textArea.position())
        return;

    auto theText = text();
    auto lines = tools::splitIntoLines(theText, textArea);
    if (lines.empty())
        return;

    auto sumUpChars = [&theText](size_t totalChars, const std::string& line) -> size_t
    {
        totalChars += line.size();
        if (theText[totalChars] == '\n')
        {
            ++totalChars;
        }
        return totalChars;
    };

    const auto cursVerIdx = to_size_t(cursorPos.y - textArea.y);
    const auto cursHorIdx = to_size_t(cursorPos.x - textArea.x);
    int charCount = std::accumulate(lines.begin(), lines.begin() + to_int(cursVerIdx), 0, sumUpChars);
    size_t deleteAt = to_size_t(charCount);
    deleteAt += cursHorIdx - 1;
    if (deleteAt >= theText.size())
        return;

    theText.erase(deleteAt, 1); 
    setText(theText);

    auto newLines = tools::splitIntoLines(theText, textArea);
    const std::string spaces(to_size_t(textArea.width), ' ');

    Position linePos { PosX(textArea.x), PosY(textArea.y + to_int(cursVerIdx)) };
    size_t lineIdx = cursVerIdx;
    if (cursVerIdx > 0)
    {
        --linePos.y;
        --lineIdx;
    }

    for (; lineIdx < newLines.size(); ++lineIdx, ++linePos.y)
    {
        curses::PrintText(spaces, getWindow(), linePos);
        curses::PrintText(newLines[lineIdx], getWindow(), linePos);
    }
    if (lines.size() > newLines.size())
    {
        curses::PrintText(spaces, getWindow(), linePos);
    }
    auto newCursorPos = cursorPos;
    if (cursHorIdx != 0)
    {
        --newCursorPos.x;
    }
    else if (cursVerIdx != 0)
    {
        --newCursorPos.y; 
        newCursorPos.x = textArea.x + len(lines[cursVerIdx-1]);
        newCursorPos.x = std::min(newCursorPos.x, textArea.x + textArea.width -1);
    }
    curses::PositionCursor(getWindow(), newCursorPos);
}

void TextEditView::setKeyFilter(const std::shared_ptr<KeyFilter>& filter)
{
    keyFilter_ = filter;
}

void TextEditView::setTextAlignment(Align) {}

namespace {
    Position mapCursorToText(const Position& cursorPos, const Rect& textArea)
    {
        auto position = textArea.position();
        position.y = cursorPos.y - textArea.y;
        position.x = cursorPos.x - textArea.x;
        return position;
    }

    size_t mapCursorPositionToTextIndex(const Position& cursorPos, const Rect& textArea, const std::string& text)
    {
        if (text.empty())
            return 0;
        
        auto sumUpChars = [&text](size_t totalChars, const std::string& line) -> size_t
        {
            totalChars += line.size();
            if (text[totalChars] == '\n')
            {
                ++totalChars;
            }
            return totalChars;
        };

        const size_t cursVerIdx = static_cast<size_t>(cursorPos.y - textArea.y);
        const size_t cursHorIdx = static_cast<size_t>(cursorPos.x - textArea.x);
        const auto lines = tools::splitIntoLines(text, textArea);
        const auto charCount = std::accumulate(lines.begin(), lines.begin() + to_int(cursVerIdx), 0, sumUpChars);
        return (to_size_t(charCount) + cursHorIdx);
    }

    int count(const std::vector<std::string>& vec)
    {
        return static_cast<int>(vec.size());
    }

    int len(const std::string& str)
    {
        return static_cast<int>(str.size());
    }

    inline int to_int(size_t value)
    {
        return static_cast<int>(value);
    }

    size_t to_size_t(int value)
    {
        return static_cast<size_t>(value);
    }
} // namespace

} // namespace ui
} // namespace memo
