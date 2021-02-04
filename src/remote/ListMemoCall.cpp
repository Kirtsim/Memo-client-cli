#include "remote/ListMemoCall.hpp"

namespace memo {
namespace remote {

ListMemoCall::ListMemoCall(model::MemoSvc::Stub& stub) :
    BaseCall(stub)
{}

ListMemoCall::ReaderPtr_t ListMemoCall::makeCall(model::MemoSvc::Stub& stub,
                                                 grpc::ClientContext& context,
                                                 grpc::CompletionQueue& completionQueue)
{
    ReaderPtr_t reader = stub.PrepareAsyncSearch(&context, request_, &completionQueue);
    return reader;
}

void ListMemoCall::setRequest(const model::MemoSearchRq& request)
{
    request_ = request;
}

const model::MemoSearchRq& ListMemoCall::request() const
{
    return request_;
}


} // namespace remote
} // namespace memo
