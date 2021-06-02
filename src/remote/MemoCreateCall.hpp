#pragma once
#include "remote/BaseCall.hpp"
#include "MemoSvc.grpc.pb.h"

namespace memo::remote {

class MemoCreateCall : public BaseCall<proto::MemoSvc::Stub, proto::MemoCreateRs>
{
public:
    using ReaderPtr_t = std::unique_ptr<grpc::ClientAsyncResponseReader<proto::MemoCreateRs>>;

    explicit MemoCreateCall(proto::MemoSvc::Stub& stub);

    void setMemo(const proto::Memo& memo);

    const proto::Memo& memo() const;

    MemoCreateCall(const MemoCreateCall&) = delete;
    MemoCreateCall& operator=(const MemoCreateCall&) = delete;

protected:
    ReaderPtr_t makeCall(proto::MemoSvc::Stub& stub,
                         grpc::ClientContext& context,
                         grpc::CompletionQueue& completionQueue) override;

private:
    proto::Memo memo_;
};

} // namespace memo