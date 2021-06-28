#include "remote/ListMemoResponseData.hpp"
#include "model/Tag.hpp"
#include "model/ModelDefs.hpp"

namespace memo::remote {

const std::string& ListMemoResponseData::requestUuid() const
{
    return requestUuid_;
}

const std::vector<model::MemoPtr>& ListMemoResponseData::memos() const
{
    return memos_;
}

const std::map<unsigned long, model::TagPtr>& ListMemoResponseData::tags() const
{
    return tags_;
}

size_t ListMemoResponseData::totalCount() const
{
    return totalCount_;
}

const std::string& ListMemoResponseData::prevPageToken() const
{
    return prevPageToken_;
}

const std::string& ListMemoResponseData::nextPageToken() const
{
    return nextPageToken_;
}

ListMemoResponseDataBuilder& ListMemoResponseDataBuilder::setRequestUuid(const std::string& requestUuid)
{
    data_.requestUuid_ = requestUuid;
    return *this;
}

ListMemoResponseDataBuilder& ListMemoResponseDataBuilder::setMemos(const std::vector<model::MemoPtr>& memos)
{
    data_.memos_ = memos;
    return *this;
}

ListMemoResponseDataBuilder& ListMemoResponseDataBuilder::setTags(const std::map<unsigned long, model::TagPtr>& tags)
{
    data_.tags_ = tags;
    return *this;
}

ListMemoResponseDataBuilder& ListMemoResponseDataBuilder::setTotalCount(size_t totalCount)
{
    data_.totalCount_ = totalCount;
    return *this;
}

ListMemoResponseDataBuilder& ListMemoResponseDataBuilder::setPrevPageToken(const std::string& prevPageToken)
{
    data_.prevPageToken_ = prevPageToken;
    return *this;
}

ListMemoResponseDataBuilder& ListMemoResponseDataBuilder::setNextPageToken(const std::string& nextPageToken)
{
    data_.nextPageToken_ = nextPageToken;
    return *this;
}

ListMemoResponseData ListMemoResponseDataBuilder::build() const
{
    return data_;
}

} // namespace memo::remote
