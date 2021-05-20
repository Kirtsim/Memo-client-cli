#pragma once
#include "utils/Structs.hpp"
#include <string>
#include <vector>

namespace memo::tools {

/// Splits the given text into lines. A section of text is split into two lines if:
/// a) There is a line feed (\n)
/// b) Word wrap is enabled and the line does not fit into the maxLineLen and must be further split
///    (wrapped on a new line)
///
/// If maxLineLen is 0 then condition b) is not taken into account, i.e. no line/word wrap. The
/// text is then split based on condition a), disregarding the maximum line length (0).
/// Empty text produces a single (empty) line.
///
/// \param text The text to be split into lines.
/// \param maxLineLen The maximum allowed length for a line.
/// \param wordWrap Flag indicating whether word wrapping should be applied
/// \return A vector of lines (strings).
std::vector<std::string> splitIntoLines(const std::string& text, size_t maxLineLen, bool wordWrap=false);

/// Splits the given text into tokens using the provided delimiter.
///
/// Ex.:
/// splitText(".abc..def") returns ["", "abc", "", "def"].
///
/// In case of an empty delimiter the entire text is returned in a vector of size 1.
///
/// \param text Text to be split
/// \param delimiter Delimiting string
/// \return Vector of tokens.
std::vector<std::string> splitText(const std::string& text, const std::string& delimiter);
} // namespace memo