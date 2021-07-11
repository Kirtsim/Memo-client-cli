#include "remote/factory/MemoCallFactory.hpp"
#include "remote/ListMemoCall.hpp"
#include "remote/ListMemosCall.hpp"
#include "remote/MemoCreateCall.hpp"
#include "remote/AddMemoCall.hpp"

namespace memo::remote {

MemoCallFactory::MemoCallFactory(const MemoSvcStubPtr_t& memoStub,
                                 const MemoServiceStubPtr_t& memoServiceStub)
    : stub_(memoStub)
    , memoServiceStub_(memoServiceStub)
{
}

std::shared_ptr<ListMemoCall> MemoCallFactory::createMemoSearchCall()
{
    if (stub_)
        return std::make_shared<ListMemoCall>(*stub_);
    return nullptr;
}

std::shared_ptr<MemoCreateCall> MemoCallFactory::createMemoCreateCall()
{
    if (stub_)
        return std::make_shared<MemoCreateCall>(*stub_);
    return nullptr;
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
