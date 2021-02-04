#pragma once
#include "model/MemoSvc.grpc.pb.h"
#include <memory>

namespace memo {
namespace remote {

class ListMemoCall;
using MemoSvcStubPtr_t = std::shared_ptr<model::MemoSvc::Stub>;

class MemoCallFactory
{
public:
    MemoCallFactory(const MemoSvcStubPtr_t& memoStub);

    std::shared_ptr<ListMemoCall> createMemoSearchCall();

private:
    MemoSvcStubPtr_t stub_;
};
} // namespace remote
} // namespace memo
