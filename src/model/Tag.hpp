#pragma once
#include "model/ModelDefs.hpp"
#include <string>

namespace memo::model {

struct Colour
{
    int red;
    int green;
    int blue;

    bool operator==(const Colour& other) const;

    bool operator!=(const Colour& other) const;
};

class Tag
{
public:
    unsigned long id() const;
    void setId(unsigned long id);

    const std::string& name() const;
    void setName(const std::string& name);

    const Colour& colour() const;
    void setColour(const Colour& colour);

    unsigned long timestamp() const;
    void setTimestamp(unsigned long timestamp);

    bool operator==(const Tag& other) const;

    bool operator!=(const Tag& other) const;

private:
    unsigned long id_;
    std::string name_;
    Colour colour_;
    unsigned long timestamp_;
};

namespace memo::utils {

/// @brief Compares the two vectors and determines if they contain the same tags.
/// Performs a deep comparison.
///
/// @return true if all tags in the first vector are present in the other vector, false otherwise.
bool CompareTags(const std::vector<model::TagPtr>& tags, const std::vector<model::TagPtr>& otherTags);

} // namespace memo::utils

} // namespace memo::model
