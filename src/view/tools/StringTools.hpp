#pragma once
#include "utils/Structs.hpp"
#include <string>
#include <vector>

namespace memo::tools {

std::vector<std::string> splitIntoLines(const std::string& text, size_t maxLineLen);

} // namespace memo