#include "view/tools/StringTools.hpp"

namespace memo::tools {

std::vector<std::string> splitIntoLines(const std::string& text, const size_t maxLineLen, const bool wordWrap)
{
    if (text.empty())
        return { "" };

    auto computeLineLength = [&](const size_t currentPos, const size_t maxEndPos)
    {
        auto actualEndPos = maxEndPos;
        if (wordWrap && maxLineLen > 0)
            actualEndPos = std::min(currentPos + maxLineLen, actualEndPos);
        return actualEndPos - currentPos;
    };

    std::vector<std::string> lines;

    const size_t textSize = text.size();
    size_t pos = 0;
    size_t endPos = text.find('\n');
    while(endPos != std::string::npos)
    {
        do
        {
            const auto lineLen = computeLineLength(pos, endPos);
            lines.emplace_back(text.substr(pos, lineLen));
            pos += lineLen;
        } while (pos < endPos);

        pos = endPos + 1;
        endPos = text.find('\n', pos);
    }
    while (pos < textSize)
    {
        const auto lineLen = computeLineLength(pos, endPos);
        lines.emplace_back(text.substr(pos, lineLen));
        pos += lineLen;
    }

    if (text.back() == '\n')
        lines.emplace_back();
    return lines;
}

} // namespace memo