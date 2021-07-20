#include "remote/ListTagsRequest.hpp"

namespace memo::remote {

const std::string& ListTagsRequest::uuid() const
{
    return uuid_;
}

const TagFilter& ListTagsRequest::filter() const
{
    return filter_;
}

int ListTagsRequest::resultPageSize() const
{
    return resultPageSize_;
}

ModelView ListTagsRequest::view() const
{
    return view_;
}

const std::string& ListTagsRequest::pageToken() const
{
    return pageToken_;
}


//////////////////////////////////////////
//        ListTagsRequestBuilder
//////////////////////////////////////////

ListTagsRequestBuilder& ListTagsRequestBuilder::setUuid(const std::string& uuid)
{
    request_.uuid_ = uuid;
    return *this;
}

ListTagsRequestBuilder& ListTagsRequestBuilder::setFilter(const TagFilter& filter)
{
    request_.filter_ = filter;
    return *this;
}

ListTagsRequestBuilder& ListTagsRequestBuilder::setResultPageSize(int pageSize)
{
    request_.resultPageSize_ = pageSize;
    return *this;
}

ListTagsRequestBuilder& ListTagsRequestBuilder::setView(ModelView view)
{
    request_.view_ = view;
    return *this;
}

ListTagsRequestBuilder& ListTagsRequestBuilder::setPageToken(const std::string& pageToken)
{
    request_.pageToken_ = pageToken;
    return *this;
}

ListTagsRequest ListTagsRequestBuilder::build() const
{
    return request_;
}
} // namespace memo::remote
