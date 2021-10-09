#pragma once
#include "remote/ServiceEnums.hpp"
#include "model/Tag.hpp"
#include <string>
#include <vector>

namespace memo::remote {

struct TagFilter {
    std::string nameStartsWith;
    std::string contains;
    std::vector<model::Color> colours;
    std::vector<unsigned long> assignedToMemos;
    unsigned long dateFrom = -1ul;
    unsigned long dateUntil = -1ul;
};

class ListTagsRequest
{
    friend class ListTagsRequestBuilder;
public:
    const std::string& uuid() const;

    const TagFilter& filter() const;

    int resultPageSize() const;

    ModelView view() const;

    const std::string& pageToken() const;

private:
    std::string uuid_;
    TagFilter filter_;
    int resultPageSize_;
    ModelView view_;
    std::string pageToken_;
};

class ListTagsRequestBuilder
{
public:
    ListTagsRequestBuilder& setUuid(const std::string& uuid);

    ListTagsRequestBuilder& setFilter(const TagFilter& filter);

    ListTagsRequestBuilder& setResultPageSize(int pageSize);

    ListTagsRequestBuilder& setView(ModelView view);

    ListTagsRequestBuilder& setPageToken(const std::string& pageToken);

    ListTagsRequest build() const;

private:
    ListTagsRequest request_;
};

} // namespace memo::remote
