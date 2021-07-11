#pragma once
#include "MemoSvc.grpc.pb.h"
#include <memory>

namespace memo::remote {

class ListMemoCall;
class ListMemosCall;
class MemoCreateCall;
class AddMemoCall;
using MemoSvcStubPtr_t = std::shared_ptr<proto::MemoSvc::Stub>;
using MemoServiceStubPtr_t = std::shared_ptr<proto::MemoService::Stub>;

class MemoCallFactory
{
public:
    MemoCallFactory(const MemoSvcStubPtr_t& memoStub,
                    const MemoServiceStubPtr_t& memoServiceStub);

    std::shared_ptr<ListMemoCall> createMemoSearchCall();

    std::shared_ptr<MemoCreateCall> createMemoCreateCall();

    std::shared_ptr<ListMemosCall> createListMemosCall();

    std::shared_ptr<AddMemoCall> createAddMemoCall();

private:
    MemoSvcStubPtr_t stub_;
    MemoServiceStubPtr_t memoServiceStub_;
};
} // namespace memo::remote
