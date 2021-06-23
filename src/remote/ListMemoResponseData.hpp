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
    void setRequestUuid(const std::string& requestUuid);

    void setMemos(const std::vector<model::MemoPtr>& memos);

    void setTags(const std::map<unsigned long, model::TagPtr>& tags);

    void setTotalCount(size_t totalCount);

    void setPrevPageToken(const std::string& prevPageToken);

    void setNextPageToken(const std::string& nextPageToken);
private:
    std::string requestUuid_;
    std::vector<model::MemoPtr> memos_;
    std::map<unsigned long, model::TagPtr> tags_;
    size_t total_count_;
    std::string prev_page_token;
    std::string next_page_token;
};

} // namespace memo::remote
