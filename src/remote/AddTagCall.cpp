#include "remote/AddTagCall.hpp"

namespace memo::remote {

AddTagCall::AddTagCall(proto::TagService::Stub& stub)
    : BaseCall(stub)
{
}

AddTagCall::~AddTagCall() = default;

void AddTagCall::setRequest(const proto::AddTagRq& request)
{
    request_ = request;
}

const proto::AddTagRq& AddTagCall::request() const
{
    return request_;
}

AddTagCall::ReaderPtr_t AddTagCall::makeCall(proto::TagService::Stub& stub, grpc::ClientContext& context,
                                             grpc::CompletionQueue& completionQueue)
{
    ReaderPtr_t reader = stub.PrepareAsyncAddTag(&context, request_, &completionQueue);
    return reader;
}
} // namespace memo::remote
