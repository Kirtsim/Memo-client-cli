#include "AddMemoCall.hpp"

namespace memo::remote {

AddMemoCall::AddMemoCall(proto::MemoService::Stub& stub)
    : BaseCall(stub)
{
}

void AddMemoCall::setRequest(const memo::proto::AddMemoRq& request)
{
    request_ = request;
}

const memo::proto::AddMemoRq& AddMemoCall::request() const
{
    return request_;
}

AddMemoCall::ReaderPtr_t AddMemoCall::makeCall(proto::MemoService::Stub& stub,
                                               grpc::ClientContext& context,
                                               grpc::CompletionQueue& completionQueue)
{
    ReaderPtr_t reader = stub.PrepareAsyncAddMemo(&context, request_, &completionQueue);
    return reader;
}
} // namespace memo::remote
