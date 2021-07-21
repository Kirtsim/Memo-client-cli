#pragma once
#include "remote/BaseCall.hpp"
#include "TagSvc.grpc.pb.h"

namespace memo::remote {


class ListTagsCall : public BaseCall<proto::TagService::Stub, proto::ListTagsRs>
{
public:
    using ReaderPtr_t = std::unique_ptr<grpc::ClientAsyncResponseReader<proto::ListTagsRs>>;

    explicit ListTagsCall(proto::TagService::Stub& stub);

    ~ListTagsCall() override;

    void setRequest(const proto::ListTagsRq& request);

    const proto::ListTagsRq& request() const;

    ListTagsCall(const ListTagsCall&) = delete;
    ListTagsCall& operator=(const ListTagsCall&) = delete;

protected:
    ReaderPtr_t makeCall(proto::TagService::Stub& stub,
                         grpc::ClientContext& context,
                         grpc::CompletionQueue& completionQueue) override;

private:
    proto::ListTagsRq request_;

};

} // namespace memo::remote
