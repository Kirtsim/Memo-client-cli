#pragma once
#include "remote/BaseCall.hpp"
#include "MemoSvc.grpc.pb.h"

namespace memo::remote {

class MemoCreateCall : public BaseCall<model::MemoSvc::Stub, model::MemoCreateRs>
{
public:
    using ReaderPtr_t = std::unique_ptr<grpc::ClientAsyncResponseReader<model::MemoCreateRs>>;

    explicit MemoCreateCall(model::MemoSvc::Stub& stub);

    void setMemo(const model::Memo& memo);

    const model::Memo& memo() const;

    MemoCreateCall(const MemoCreateCall&) = delete;
    MemoCreateCall& operator=(const MemoCreateCall&) = delete;

protected:
    ReaderPtr_t makeCall(model::MemoSvc::Stub& stub,
                         grpc::ClientContext& context,
                         grpc::CompletionQueue& completionQueue) override;

private:
    model::Memo memo_;
};

} // namespace memo