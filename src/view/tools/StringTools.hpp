#pragma once
#include "utils/Structs.hpp"
#include <string>
#include <vector>

namespace memo::tools {

/// Splits the given text into lines. A section of text is split into two lines if:
/// a) There is a line feed (\n)
/// b) The line does not fit into the maxLineLen and must be further split (wrapped on a new line)
///
/// If maxLineLen is 0 then condition b) is not taken into account, i.e. no line/word wrap.
/// Empty text produces a single (empty) line. An extra empty line is added to the end in a scenario
/// when the last line has the same length as the maxLineLen.
///
/// \param text The text to be split into lines.
/// \param maxLineLen The maximum allowed length for a line.
/// \return A vector of lines (strings).
std::vector<std::string> splitIntoLines(const std::string& text, size_t maxLineLen);

} // namespace memo