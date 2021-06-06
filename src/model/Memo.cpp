#include "Memo.hpp"

namespace memo::model {

unsigned long Memo::id() const
{
    return id_;
}

const std::string& Memo::title() const
{
    return title_;
}

const std::string& Memo::description() const
{
    return description_;
}

const std::vector<std::string>& Memo::tagNames() const
{
    return tagNames_;
}

unsigned long Memo::timestamp() const
{
    return timestamp_;
}

void Memo::setId(const unsigned long id)
{
    id_ = id;
}

void Memo::setTitle(const std::string& title)
{
    title_ = title;
}

void Memo::setDescription(const std::string& description)
{
    description_ = description;
}

void Memo::setTagNames(const std::vector<std::string>& tagNames)
{
    tagNames_ = tagNames;
}

void Memo::addTagName(const std::string& tagName)
{
    tagNames_.emplace_back(tagName);
}

void Memo::setTimestamp(const unsigned long timestamp)
{
    timestamp_ = timestamp;
}

} // namespace memo::model