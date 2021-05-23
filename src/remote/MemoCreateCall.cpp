#include "MemoCreateCall.hpp"

namespace memo::remote {

MemoCreateCall::MemoCreateCall(model::MemoSvc::Stub& stub) :
        BaseCall(stub)
{}

MemoCreateCall::ReaderPtr_t MemoCreateCall::makeCall(model::MemoSvc::Stub& stub,
                                                 grpc::ClientContext& context,
                                                 grpc::CompletionQueue& completionQueue)
{
    ReaderPtr_t reader = stub.PrepareAsyncCreate(&context, memo_, &completionQueue);
    return reader;
}

void MemoCreateCall::setMemo(const model::Memo& memo)
{
    memo_ = memo;
}

const model::Memo& MemoCreateCall::memo() const
{
    return memo_;
}

} // namespace memo::remote
