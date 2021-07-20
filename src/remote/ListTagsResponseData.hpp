#pragma once
#include "model/ModelDefs.hpp"
#include <string>
#include <vector>

namespace memo::remote {


class ListTagsResponseData
{
    friend class ListTagsResponseDataBuilder;
public:
    const std::string& requestUuid() const;

    const std::vector<model::TagPtr>& tags() const;

    int totalCount() const;

    const std::string& prevPageToken() const;

    const std::string& nextPageToken() const;

private:
    std::string requestUuid_;
    std::vector<model::TagPtr> tags_;
    int totalCount_ = 0;
    std::string prevPageToken_;
    std::string nextPageToken_;
};

class ListTagsResponseDataBuilder
{
public:
    ListTagsResponseDataBuilder& setRequestUuid(const std::string& uuid);

    ListTagsResponseDataBuilder& setTags(const std::vector<model::TagPtr>& tags);

    ListTagsResponseDataBuilder& addTag(const model::TagPtr& tag);

    ListTagsResponseDataBuilder& setTotalCount(int totalCount);

    ListTagsResponseDataBuilder& setPrevPageToken(const std::string& prevPageToken);

    ListTagsResponseDataBuilder& setNextPageToken(const std::string& nextPageToken);

    ListTagsResponseData build() const;

private:
    ListTagsResponseData data_;
};



} // namespace memo::remote
