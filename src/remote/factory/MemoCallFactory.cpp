#include "remote/factory/MemoCallFactory.hpp"
#include "remote/ListMemoCall.hpp"
#include "remote/MemoCreateCall.hpp"

namespace memo::remote {

MemoCallFactory::MemoCallFactory(const MemoSvcStubPtr_t& memoStub)
    : stub_(memoStub)
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

} // namespace memo::remote
