#pragma once
#include "model/ModelDefs.hpp"

#include <string>
#include <vector>
#include <map>


namespace memo::remote {

class ListMemoResponseData;

class ListMemoResponseData
{
    friend class ListMemoResponseDataBuilder;

public:
    const std::string& requestUuid() const;

    const std::vector<model::MemoPtr>& memos() const;

    const std::map<unsigned long, model::TagPtr>& tags() const;

    size_t totalCount() const;

    const std::string& prevPageToken() const;

    const std::string& nextPageToken() const;

private:
    std::string requestUuid_;
    std::vector<model::MemoPtr> memos_;
    std::map<unsigned long, model::TagPtr> tags_;
    size_t totalCount_;
    std::string prevPageToken_;
    std::string nextPageToken_;
};

class ListMemoResponseDataBuilder
{
public:
    ~ListMemoResponseDataBuilder() = default;

    ListMemoResponseDataBuilder& setRequestUuid(const std::string& requestUuid);

    ListMemoResponseDataBuilder& setMemos(const std::vector<model::MemoPtr>& memos);

    ListMemoResponseDataBuilder& setTags(const std::map<unsigned long, model::TagPtr>& tags);

    ListMemoResponseDataBuilder& setTotalCount(size_t totalCount);

    ListMemoResponseDataBuilder& setPrevPageToken(const std::string& prevPageToken);

    ListMemoResponseDataBuilder& setNextPageToken(const std::string& nextPageToken);

    ListMemoResponseData build() const;

private:
    ListMemoResponseData data_;
};

} // namespace memo::remote
