#pragma once
#include "remote/Call.hpp"

#include <grpcpp/impl/codegen/completion_queue.h>
#include <grpcpp/grpcpp.h>
#include <memory>

namespace memo {
namespace remote {

template<class Stub, class Reply>
class BaseCall : public Call
{
public:
    using ReaderPtr_t = std::unique_ptr<grpc::ClientAsyncResponseReader<Reply>>;

    BaseCall(Stub& stub);
    virtual ~BaseCall();

    bool exec() override;
    grpc::Status getStatus() const override;
    const Reply& getReply() const;

protected:
    virtual ReaderPtr_t makeCall(Stub& stub, grpc::ClientContext& context,
                                 grpc::CompletionQueue& completionQueue) = 0;
private:
    Stub& stub_;
    Reply reply_;
    grpc::ClientContext context_;
    grpc::CompletionQueue completionQueue_;
    grpc::Status status_;
    ReaderPtr_t reader_;
};

template<class Stub, class Reply>
BaseCall<Stub, Reply>::BaseCall(Stub& stub) :
    stub_(stub)
{}

template<class Stub, class Reply>
BaseCall<Stub, Reply>::~BaseCall()
{
    completionQueue_.Shutdown();

    void* ignoredTag;
    bool  ignoredStatus;
    completionQueue_.Next(&ignoredTag, &ignoredStatus);
}

template<class Stub, class Reply>
bool BaseCall<Stub, Reply>::exec()
{
    reader_ = makeCall(stub_, context_, completionQueue_);
    if (!reader_)
    {
        std::cerr << "Reader is null. Terminating program." << std::endl;
        std::exit(1);
    }
    reader_->StartCall();
    reader_->Finish(&reply_, &status_, (void*)this);

    void* tag;
    bool status = false;

    // block the main thread until a response arrives
    completionQueue_.Next(&tag, &status);
    if (!status)
        std::cerr << "GRPC failed." << std::endl;
    return status;
}

template<class Stub, class Reply>
grpc::Status BaseCall<Stub, Reply>::getStatus() const
{
    return status_;
}

template<class Stub, class Reply>
const Reply& BaseCall<Stub, Reply>::getReply() const
{
    return reply_;
}

} // namespace remote
} // namespace memo
