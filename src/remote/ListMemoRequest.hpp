#pragma once
#include "remote/ServiceEnums.hpp"
#include <string>
#include <vector>

namespace memo::remote {

struct MemoFilter
{
    std::string titleStartsWith;
    std::string titleContains;
    std::string contains;
    std::vector<unsigned long> taggedBy_;
    unsigned long dateFrom;
    unsigned long dateUntil;
};

enum MemoView
{
    MINIMAL,     // Memo will contain Id, Title and timestamp
    COMPLETE     // Memo will contain all of its fields
};

class ListMemoRequest
{
    friend class ListMemoRequestBuilder;

public:
    const std::string& uuid() const;

    const MemoFilter& filter() const;

    int pageSize() const;

    ModelView memoView() const;

    const std::string& pageToken() const;

private:
    void setUuid(const std::string& uuid);

    void setFilter(const MemoFilter& filter);

    void setPageSize(int pageSize);

    void setMemoView(ModelView memoView);

    void setPageToken(const std::string& pageToken);

private:
    std::string uuid_;
    MemoFilter filter_;
    int pageSize_;
    ModelView memoView_;
    std::string pageToken_;
};

class ListMemoRequestBuilder
{
public:
    ListMemoRequestBuilder& setRequestId(const std::string& uuid);

    ListMemoRequestBuilder& setFilter(const MemoFilter& filter);

    ListMemoRequestBuilder& setPageSize(int pageSize);

    ListMemoRequestBuilder& setMemoView(ModelView memoView);

    ListMemoRequestBuilder& setPageToken(const std::string& pageToken);

    ListMemoRequest build() const;

private:
    ListMemoRequest request_;
};

} // namespace memo::remote
