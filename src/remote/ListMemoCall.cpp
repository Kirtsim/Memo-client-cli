#include "remote/ListMemoCall.hpp"

namespace memo {
namespace remote {

ListMemoCall::ListMemoCall(model::MemoSvc::Stub& ioStub) :
    BaseCall(ioStub)
{}

ListMemoCall::ReaderPtr_t ListMemoCall::makeCall(model::MemoSvc::Stub& iStub,
                                                 grpc::ClientContext& iContext,
                                                 grpc::CompletionQueue& iCompletionQueue)
{
    ReaderPtr_t reader = iStub.PrepareAsyncSearch(&iContext, request_, &iCompletionQueue);
    return reader;
}

} // namespace remote
} // namespace memo
