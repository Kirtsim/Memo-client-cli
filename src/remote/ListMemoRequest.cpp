#include "ListMemoRequest.hpp"

namespace memo::remote {

const std::string& ListMemoRequest::uuid() const
{
    return uuid_;
}

const MemoFilter& ListMemoRequest::filter() const
{
    return filter_;
}

int ListMemoRequest::pageSize() const
{
    return pageSize_;
}

MemoView ListMemoRequest::memoView() const
{
    return memoView_;
}

const std::string& ListMemoRequest::pageToken() const
{
    return pageToken_;
}

void ListMemoRequest::setUuid(const std::string& uuid)
{
    uuid_ = uuid;
}

void ListMemoRequest::setFilter(const MemoFilter& filter)
{
    filter_ = filter;
}

void ListMemoRequest::setPageSize(int pageSize)
{
    pageSize_ = pageSize;
}

void ListMemoRequest::setMemoView(MemoView memoView)
{
    memoView_ = memoView;
}

void ListMemoRequest::setPageToken(const std::string& pageToken)
{
    pageToken_ = pageToken;
}

//////////////////////////////////////////////////////////////////////////////////////////
///                         ListMemoRequestBuilder
//////////////////////////////////////////////////////////////////////////////////////////

ListMemoRequestBuilder& ListMemoRequestBuilder::setRequestId(const std::string& uuid)
{
    request_.setUuid(uuid);
    return *this;
}

ListMemoRequestBuilder& ListMemoRequestBuilder::setFilter(const MemoFilter& filter)
{
    request_.setFilter(filter);
    return *this;
}

ListMemoRequestBuilder& ListMemoRequestBuilder::setPageSize(int pageSize)
{
    request_.setPageSize(pageSize);
    return *this;
}

ListMemoRequestBuilder& ListMemoRequestBuilder::setMemoView(MemoView memoView)
{
    request_.setMemoView(memoView);
    return *this;
}

ListMemoRequestBuilder& ListMemoRequestBuilder::setPageToken(const std::string& pageToken)
{
    request_.setPageToken(pageToken);
    return *this;
}

ListMemoRequest ListMemoRequestBuilder::build() const
{
    return request_;
}

} // namespace memo::remote
