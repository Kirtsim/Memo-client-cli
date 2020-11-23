#pragma once
#include "remote/BaseCall.hpp"
#include "model/MemoSvc.grpc.pb.h"

namespace memo {
namespace remote {

class ListMemoCall : public BaseCall<model::MemoSvc::Stub, model::MemoSearchRs>
{
public:
    using ReaderPtr_t = std::unique_ptr<grpc::ClientAsyncResponseReader<model::MemoSearchRs>>;

    ListMemoCall(model::MemoSvc::Stub& ioStub);

    ListMemoCall(const ListMemoCall&) = delete;
    ListMemoCall& operator=(const ListMemoCall&) = delete;

protected:
    ReaderPtr_t makeCall(model::MemoSvc::Stub& iStub,
                         grpc::ClientContext& iContext,
                         grpc::CompletionQueue& iCompletionQueue) override;

private:
    model::MemoSearchRq request_;
};

} // namespace remote
} // namespace memo
