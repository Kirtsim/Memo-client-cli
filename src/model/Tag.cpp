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

const std::string& Tag::color() const
{
    return color_;
}

unsigned long Tag::timestamp() const
{
    return timestamp_;
}

void Tag::setName(const std::string& name)
{
    name_ = name;
}

void Tag::setColor(const std::string& color)
{
    color_ = color;
}

void Tag::setTimestamp(unsigned long timestamp)
{
    timestamp_ = timestamp;
}

} // namespace memo::model
