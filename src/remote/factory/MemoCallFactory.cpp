#include "remote/factory/MemoCallFactory.hpp"
#include "remote/ListMemoCall.hpp"

namespace memo {
namespace remote {

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


} // namespace remote
} // namespace memo
