#pragma once
#include "remote/BaseCall.hpp"
#include "MemoSvc.grpc.pb.h"

namespace memo::remote {

class ListMemoCall : public BaseCall<proto::MemoSvc::Stub, proto::MemoSearchRs>
{
public:
    using ReaderPtr_t = std::unique_ptr<grpc::ClientAsyncResponseReader<proto::MemoSearchRs>>;

    explicit ListMemoCall(proto::MemoSvc::Stub& stub);

    void setRequest(const proto::MemoSearchRq& request);

    const proto::MemoSearchRq& request() const;

    ListMemoCall(const ListMemoCall&) = delete;
    ListMemoCall& operator=(const ListMemoCall&) = delete;

protected:
    ReaderPtr_t makeCall(proto::MemoSvc::Stub& stub,
                         grpc::ClientContext& context,
                         grpc::CompletionQueue& completionQueue) override;

private:
    proto::MemoSearchRq request_;
};

} // namespace memo
