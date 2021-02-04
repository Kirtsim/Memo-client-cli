#pragma once
#include "remote/BaseCall.hpp"
#include "model/MemoSvc.grpc.pb.h"

namespace memo {
namespace remote {

class ListMemoCall : public BaseCall<model::MemoSvc::Stub, model::MemoSearchRs>
{
public:
    using ReaderPtr_t = std::unique_ptr<grpc::ClientAsyncResponseReader<model::MemoSearchRs>>;

    ListMemoCall(model::MemoSvc::Stub& stub);

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

} // namespace remote
} // namespace memo
