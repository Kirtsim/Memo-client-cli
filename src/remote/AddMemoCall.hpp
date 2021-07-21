#pragma once
#include "remote/BaseCall.hpp"
#include "MemoSvc.grpc.pb.h"

namespace memo::remote {

class AddMemoCall : public BaseCall<proto::MemoService::Stub, proto::AddMemoRs>
{
public:
    using ReaderPtr_t = std::unique_ptr<grpc::ClientAsyncResponseReader<proto::AddMemoRs>>;

    explicit AddMemoCall(proto::MemoService::Stub& stub);

    void setRequest(const proto::AddMemoRq& request);

    const proto::AddMemoRq& request() const;

    AddMemoCall(const AddMemoCall&) = delete;
    AddMemoCall& operator=(const AddMemoCall&) = delete;

protected:
    ReaderPtr_t makeCall(proto::MemoService::Stub& stub,
                         grpc::ClientContext& context,
                         grpc::CompletionQueue& completionQueue) override;

private:
    proto::AddMemoRq request_;
};

} // namespace memo::remote
