#include "remote/factory/MemoCallFactory.hpp"
#include "remote/ListMemosCall.hpp"
#include "remote/AddMemoCall.hpp"

namespace memo::remote {

MemoCallFactory::MemoCallFactory(const MemoServiceStubPtr_t& memoServiceStub)
    : memoServiceStub_(memoServiceStub)
{
}

std::shared_ptr<ListMemosCall> MemoCallFactory::createListMemosCall()
{
    if (memoServiceStub_)
        return std::make_shared<ListMemosCall>(*memoServiceStub_);
    return nullptr;
}

std::shared_ptr<AddMemoCall> MemoCallFactory::createAddMemoCall()
{
    if (memoServiceStub_)
        return std::make_shared<AddMemoCall>(*memoServiceStub_);
    return nullptr;
}

} // namespace memo::remote
