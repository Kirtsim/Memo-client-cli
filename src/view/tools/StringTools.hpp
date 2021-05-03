#pragma once
#include "utils/Structs.hpp"
#include <string>
#include <vector>

namespace memo {
namespace tools {

std::vector<std::string> splitIntoLines(const std::string& text, const Rect& textArea);

} // namespace tools
} // namespace memo