#pragma once
#include "MemoSvc.grpc.pb.h"
#include <memory>

namespace memo::remote {

class ListMemoCall;
class MemoCreateCall;
using MemoSvcStubPtr_t = std::shared_ptr<proto::MemoSvc::Stub>;

class MemoCallFactory
{
public:
    explicit MemoCallFactory(const MemoSvcStubPtr_t& memoStub);

    std::shared_ptr<ListMemoCall> createMemoSearchCall();

    std::shared_ptr<MemoCreateCall> createMemoCreateCall();

private:
    MemoSvcStubPtr_t stub_;
};
} // namespace memo::remote
