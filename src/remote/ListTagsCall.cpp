#include "remote/ListTagsCall.hpp"

namespace memo::remote {

ListTagsCall::ListTagsCall(proto::TagService::Stub& stub)
    : BaseCall(stub)
{
}

ListTagsCall::~ListTagsCall() = default;

void ListTagsCall::setRequest(const proto::ListTagsRq& request)
{
    request_ = request;
}

const proto::ListTagsRq& ListTagsCall::request() const
{
    return request_;
}

ListTagsCall::ReaderPtr_t ListTagsCall::makeCall(
        proto::TagService::Stub& stub,
        grpc::ClientContext& context,
        grpc::CompletionQueue& completionQueue)
{
    ReaderPtr_t reader = stub.PrepareAsyncListTags(&context, request_, &completionQueue);
    return reader;
}

} // namespace memo::remote
