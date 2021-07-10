#include "model/Memo.hpp"
#include "model/Tag.hpp"
#include <set>

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

const std::vector<TagPtr>& Memo::tags() const
{
    return tags_;
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

void Memo::setTags(const std::vector<TagPtr>& tags)
{
    tags_ = tags;
}

void Memo::addTag(const TagPtr& tagName)
{
    tags_.emplace_back(tagName);
}

void Memo::setTimestamp(const unsigned long timestamp)
{
    timestamp_ = timestamp;
}

bool Memo::operator==(const Memo& other) const
{
    return id_ == other.id_
        && timestamp_ == other.timestamp_
        && title_ == other.title_
        && description_ == other.description_
        && memo::utils::CompareTags(tags_, other.tags_);
}

bool Memo::operator!=(const Memo& other) const
{
    return !(*this == other);
}
} // namespace memo::model