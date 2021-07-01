#include "remote/ListMemosCall.hpp"

namespace memo::remote {

ListMemosCall::ListMemosCall(proto::MemoService::Stub& stub)
    : BaseCall(stub)
{
}

void ListMemosCall::setRequest(const proto::ListMemosRq& request)
{
    request_ = request;
}

const memo::proto::ListMemosRq& ListMemosCall::request() const
{
    return request_;
}

ListMemosCall::ReaderPtr_t ListMemosCall::makeCall(proto::MemoService::Stub& stub,
                                                   grpc::ClientContext& context,
                                                   grpc::CompletionQueue& completionQueue)
{
    ReaderPtr_t reader = stub.PrepareAsyncListMemos(&context, request_, &completionQueue);
    return reader;
}
} // namespace memo::remote
