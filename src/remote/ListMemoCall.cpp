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

} // namespace remote
} // namespace memo
