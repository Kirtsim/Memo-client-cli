#pragma once
#include "remote/BaseCall.hpp"
#include "MemoSvc.grpc.pb.h"

namespace memo::remote {

class ListMemosCall : public BaseCall<proto::MemoService::Stub, proto::ListMemosRs>
{
public:
    using ReaderPtr_t = std::unique_ptr<grpc::ClientAsyncResponseReader<proto::ListMemosRs>>;

    explicit ListMemosCall(proto::MemoService::Stub& stub);

    void setRequest(const proto::ListMemosRq& request);

    const proto::ListMemosRq& request() const;

    ListMemosCall(const ListMemosCall&) = delete;
    ListMemosCall& operator=(const ListMemosCall&) = delete;

protected:
    ReaderPtr_t makeCall(proto::MemoService::Stub& stub,
                         grpc::ClientContext& context,
                         grpc::CompletionQueue& completionQueue) override;

private:
    proto::ListMemosRq request_;
};

} // namespace memo::remote
