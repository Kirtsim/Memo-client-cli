#include "remote/AddMemoResponseData.hpp"

namespace memo::remote {

const std::string& AddMemoResponseData::requestUuid() const
{
    return requestUuid_;
}

void AddMemoResponseData::setRequestUuid(const std::string& requestUuid)
{
    requestUuid_ = requestUuid;
}

const model::MemoPtr& AddMemoResponseData::memo() const
{
    return memo_;
}

void AddMemoResponseData::setMemo(const memo::model::MemoPtr& memo)
{
    memo_ = memo;
}

} // namespace memo::remote
