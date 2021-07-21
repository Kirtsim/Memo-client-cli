#include "remote/ListTagsResponseData.hpp"

namespace memo::remote {

const std::string& ListTagsResponseData::requestUuid() const
{
    return requestUuid_;
}

const std::vector<model::TagPtr>& ListTagsResponseData::tags() const
{
    return tags_;
}

int ListTagsResponseData::totalCount() const
{
    return totalCount_;
}

const std::string& ListTagsResponseData::prevPageToken() const
{
    return prevPageToken_;
}

const std::string& ListTagsResponseData::nextPageToken() const
{
    return nextPageToken_;
}

ListTagsResponseDataBuilder& ListTagsResponseDataBuilder::setRequestUuid(const std::string& uuid)
{
    data_.requestUuid_ = uuid;
    return *this;
}

ListTagsResponseDataBuilder& ListTagsResponseDataBuilder::setTags(const std::vector<model::TagPtr>& tags)
{
    data_.tags_ = tags;
    return *this;
}

ListTagsResponseDataBuilder& ListTagsResponseDataBuilder::addTag(const model::TagPtr& tag)
{
    data_.tags_.emplace_back(tag);
    return *this;
}

ListTagsResponseDataBuilder& ListTagsResponseDataBuilder::setTotalCount(int totalCount)
{
    data_.totalCount_ = totalCount;
    return *this;
}

ListTagsResponseDataBuilder& ListTagsResponseDataBuilder::setPrevPageToken(const std::string& prevPageToken)
{
    data_.prevPageToken_ = prevPageToken;
    return *this;
}

ListTagsResponseDataBuilder& ListTagsResponseDataBuilder::setNextPageToken(const std::string& nextPageToken)
{
    data_.nextPageToken_ = nextPageToken;
    return *this;
}

ListTagsResponseData ListTagsResponseDataBuilder::build() const
{
    return data_;
}

} // namespace memo::remote