#include "Tag.hpp"
#include <vector>

namespace memo::model {

unsigned long Tag::id() const
{
    return id_;
}

void Tag::setId(unsigned long id)
{
    id_ = id;
}

const std::string& Tag::name() const
{
    return name_;
}

const Colour& Tag::colour() const
{
    return colour_;
}

unsigned long Tag::timestamp() const
{
    return timestamp_;
}

void Tag::setName(const std::string& name)
{
    name_ = name;
}

void Tag::setColour(const Colour& colour)
{
    colour_ = colour;
}

void Tag::setTimestamp(unsigned long timestamp)
{
    timestamp_ = timestamp;
}

bool Tag::operator==(const Tag& other) const
{
    return other.id_ == id_
        && other.name_ == name_
        && other.colour_ == colour_
        && other.timestamp_ == timestamp_;
}

bool Tag::operator!=(const Tag& other) const
{
    return !(*this == other);
}

bool Colour::operator==(const Colour& other) const
{
    return other.red == red
        && other.green == green
        && other.blue == blue;
}

bool Colour::operator!=(const Colour& other) const
{
    return !(*this == other);
}

} // namespace memo::model

namespace memo::utils {

bool CompareTags(const std::vector<model::TagPtr>& tags, const std::vector<model::TagPtr>& otherTags)
{
    if (tags.size() != otherTags.size())
        return false;

    auto idLessThan = [](const model::TagPtr& tagA, const model::TagPtr& tagB) -> bool
    {
        return tagA && (!tagB || tagA->id() < tagB->id());
    };
    std::vector<model::TagPtr> tagsCopy(tags.begin(), tags.end());
    std::vector<model::TagPtr> tagsOtherCopy(otherTags.begin(), otherTags.end());
    std::sort(tagsCopy.begin(), tagsCopy.end(), idLessThan);
    std::sort(tagsOtherCopy.begin(), tagsOtherCopy.end(), idLessThan);
    for (size_t i = 0; i < tagsCopy.size(); ++i)
    {
        const auto& tag = tagsCopy[i];
        const auto& otherTag = tagsOtherCopy[i];
        if ((!tag && otherTag) || (tag && !otherTag))
            return false;
        if (tag && *tag != *otherTag)
            return false;
    }
    return true;
}

} // namespace memo::utils
