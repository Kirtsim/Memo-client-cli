#pragma once
#include "remote/BaseCall.hpp"
#include "TagSvc.grpc.pb.h"

namespace memo::remote {

class AddTagCall : public BaseCall<proto::TagService::Stub, proto::AddTagRs>
{
public:
    using ReaderPtr_t = std::unique_ptr<grpc::ClientAsyncResponseReader<proto::AddTagRs>>;

    explicit AddTagCall(proto::TagService::Stub& stub);

    ~AddTagCall() override;

    void setRequest(const proto::AddTagRq& request);

    const proto::AddTagRq& request() const;

    AddTagCall(const AddTagCall&) = delete;
    AddTagCall& operator=(const AddTagCall&) = delete;

protected:
    ReaderPtr_t makeCall(proto::TagService::Stub& stub,
                         grpc::ClientContext& context,
                         grpc::CompletionQueue& completionQueue) override;

private:
    proto::AddTagRq request_;
};

} // namespace memo::remote
