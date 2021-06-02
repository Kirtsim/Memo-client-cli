#include "remote/ListMemoCall.hpp"

namespace memo::remote {

ListMemoCall::ListMemoCall(proto::MemoSvc::Stub& stub) :
    BaseCall(stub)
{}

ListMemoCall::ReaderPtr_t ListMemoCall::makeCall(proto::MemoSvc::Stub& stub,
                                                 grpc::ClientContext& context,
                                                 grpc::CompletionQueue& completionQueue)
{
    ReaderPtr_t reader = stub.PrepareAsyncSearch(&context, request_, &completionQueue);
    return reader;
}

void ListMemoCall::setRequest(const proto::MemoSearchRq& request)
{
    request_ = request;
}

const proto::MemoSearchRq& ListMemoCall::request() const
{
    return request_;
}


} // namespace memo::remote
