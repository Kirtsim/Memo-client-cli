#include "remote/AddMemoResponseData.hpp"

namespace memo::remote {

const std::string& AddMemoResponseData::requestUuid() const
{
    return requestUuid_;
}

const model::MemoPtr& AddMemoResponseData::memo() const
{
    return memo_;
}

AddMemoResponseDataBuilder& AddMemoResponseDataBuilder::setRequestUuid(const std::string& requestUuid)
{
    data_.requestUuid_ = requestUuid;
    return *this;
}

AddMemoResponseDataBuilder& AddMemoResponseDataBuilder::setMemo(const model::MemoPtr& memo)
{
    data_.memo_ = memo;
    return *this;
}

AddMemoResponseData AddMemoResponseDataBuilder::build() const
{
    return data_;
}

} // namespace memo::remote
