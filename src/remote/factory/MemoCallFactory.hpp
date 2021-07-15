#pragma once
#include "MemoSvc.grpc.pb.h"
#include <memory>

namespace memo::remote {

class ListMemosCall;
class AddMemoCall;
using MemoServiceStubPtr_t = std::shared_ptr<proto::MemoService::Stub>;

class MemoCallFactory
{
public:
    explicit MemoCallFactory(const MemoServiceStubPtr_t& memoServiceStub);

    std::shared_ptr<ListMemosCall> createListMemosCall();

    std::shared_ptr<AddMemoCall> createAddMemoCall();

private:
    MemoServiceStubPtr_t memoServiceStub_;
};
} // namespace memo::remote
