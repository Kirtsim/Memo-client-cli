#include "MemoCreateCall.hpp"

namespace memo::remote {

MemoCreateCall::MemoCreateCall(proto::MemoSvc::Stub& stub) :
        BaseCall(stub)
{}

MemoCreateCall::ReaderPtr_t MemoCreateCall::makeCall(proto::MemoSvc::Stub& stub,
                                                 grpc::ClientContext& context,
                                                 grpc::CompletionQueue& completionQueue)
{
    ReaderPtr_t reader = stub.PrepareAsyncCreate(&context, memo_, &completionQueue);
    return reader;
}

void MemoCreateCall::setMemo(const proto::Memo& memo)
{
    memo_ = memo;
}

const proto::Memo& MemoCreateCall::memo() const
{
    return memo_;
}

} // namespace memo::remote
