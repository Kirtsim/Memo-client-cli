#include "remote/ListMemoResponseData.hpp"
#include "model/Tag.hpp"
#include "model/ModelDefs.hpp"

namespace memo::remote {

const std::string& ListMemoResponseData::requestUuid() const
{
    return requestUuid_;
}

void ListMemoResponseData::setRequestUuid(const std::string& requestUuid)
{
    requestUuid_ = requestUuid;
}

const std::vector<model::MemoPtr>& ListMemoResponseData::memos() const
{
    return memos_;
}

void ListMemoResponseData::setMemos(const std::vector<model::MemoPtr>& memos)
{
    memos_ = memos;
}

const std::map<unsigned long, model::TagPtr>& ListMemoResponseData::tags() const
{
    return tags_;
}

void ListMemoResponseData::setTags(const std::map<unsigned long, model::TagPtr>& tags)
{
    tags_ = tags;
}

size_t ListMemoResponseData::totalCount() const
{
    return total_count_;
}

void ListMemoResponseData::setTotalCount(size_t totalCount)
{
    total_count_ = totalCount;
}

const std::string& ListMemoResponseData::prevPageToken() const
{
    return prev_page_token;
}

void ListMemoResponseData::setPrevPageToken(const std::string& prevPageToken)
{
    prev_page_token = prevPageToken;
}

const std::string& ListMemoResponseData::nextPageToken() const
{
    return next_page_token;
}

void ListMemoResponseData::setNextPageToken(const std::string& nextPageToken)
{
    next_page_token = nextPageToken;
}

} // namespace memo::remote
