#pragma once
#include "remote/BaseCall.hpp"
#include "MemoSvc.grpc.pb.h"

namespace memo::remote {

class ListMemoCall : public BaseCall<model::MemoSvc::Stub, model::MemoSearchRs>
{
public:
    using ReaderPtr_t = std::unique_ptr<grpc::ClientAsyncResponseReader<model::MemoSearchRs>>;

    explicit ListMemoCall(model::MemoSvc::Stub& stub);

    void setRequest(const model::MemoSearchRq& request);

    const model::MemoSearchRq& request() const;

    ListMemoCall(const ListMemoCall&) = delete;
    ListMemoCall& operator=(const ListMemoCall&) = delete;

protected:
    ReaderPtr_t makeCall(model::MemoSvc::Stub& stub,
                         grpc::ClientContext& context,
                         grpc::CompletionQueue& completionQueue) override;

private:
    model::MemoSearchRq request_;
};

} // namespace memo
