#include "Tag.hpp"

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
