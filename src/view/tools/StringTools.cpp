#include "view/tools/StringTools.hpp"

namespace memo {
namespace tools {
    
std::vector<std::string> splitIntoLines(const std::string& text, const Rect& textArea)
{
    if (text.empty())
        return { "" };

    std::vector<std::string> lines;

    const size_t maxLineLen = static_cast<size_t>(textArea.width);
    const size_t textSize = text.size();
    size_t pos = 0;
    size_t endPos = text.find('\n');
    while(endPos != std::string::npos)
    {
        do
        {
            auto lineLen = std::min(pos + maxLineLen, endPos) - pos;
            lines.emplace_back(text.substr(pos, lineLen));
            pos += lineLen;
        } while (pos < endPos);
        if (lines.back().size() == maxLineLen)
        {
            lines.emplace_back();
        }
        
        pos = endPos + 1;
        endPos = text.find('\n', pos);
    }
    while (pos < textSize)
    {
        auto lineLen = std::min(pos + maxLineLen, textSize) - pos;
        lines.emplace_back(text.substr(pos, lineLen));
        pos += lineLen;
    }
    if (text.back() == '\n' || lines.back().size() == maxLineLen)
        lines.emplace_back();
    return lines;
}

} // namespace tools
} // namespace memo