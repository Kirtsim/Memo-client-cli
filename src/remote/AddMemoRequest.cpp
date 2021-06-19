#include "AddMemoRequest.hpp"

namespace memo::remote {

const std::string& AddMemoRequest::uuid() const
{
    return uuid_;
}

const model::MemoPtr& AddMemoRequest::memo() const
{
    return memo_;
}

void AddMemoRequest::setUuid(const std::string& uuid)
{
    uuid_ = uuid;
}

void AddMemoRequest::setMemo(const model::MemoPtr& memo)
{
    memo_ = memo;
}


//////////////////////////////////////////////////////////////////////////////////////////
///                         AddMemoRequestBuilder
//////////////////////////////////////////////////////////////////////////////////////////


AddMemoRequestBuilder& AddMemoRequestBuilder::setRequestUuid(const std::string& uuid)
{
    request_.setUuid(uuid);
    return *this;
}

AddMemoRequestBuilder& AddMemoRequestBuilder::setMemo(const model::MemoPtr& memo)
{
    request_.setMemo(memo);
    return *this;
}

AddMemoRequest AddMemoRequestBuilder::build()
{
    return request_;
}

} // namespace memo::remote
