#include "Client.hpp"
#include "model/MemoSvc.grpc.pb.h"
#include "model/TagSvc.grpc.pb.h"

#include <iostream>
#include <memory>

#include <grpcpp/grpcpp.h>
#include <grpc/support/log.h>
#include <thread>

namespace memo {
namespace client {

class Call
{
public:
    virtual ~Call() = default;
    virtual void exec() = 0;
    virtual void registerIn(grpc::CompletionQueue* icompletionQueue) = 0;
    virtual void startCall() = 0;

    virtual grpc::Status getStatus() const = 0;
};

template<class Stub, class Reply>
class BaseCall : public Call
{
public:
    BaseCall(Stub& ioStub) :
        stub_(ioStub) {}
    virtual ~BaseCall() = default;
    void startCall() override
    {
        reader->StartCall();
        reader->Finish(&reply, &status_, (void*)this);
    }

    grpc::Status getStatus() const override
    {
        return status_;
    }
protected:
    Stub& stub_;
    Reply reply;
    grpc::ClientContext context;
    grpc::Status status_;
    std::unique_ptr<grpc::ClientAsyncResponseReader<Reply>> reader;
};

class MemoSearchCall : public BaseCall<model::MemoSvc::Stub, model::MemoSearchRs>
{
public:
    MemoSearchCall(model::MemoSvc::Stub& ioStub, model::MemoSearchRq iRequest) :
        BaseCall(ioStub), request_(iRequest) {}
    ~MemoSearchCall() = default;

    void registerIn(grpc::CompletionQueue* iCompletionQueue) override
    {
        reader = stub_.PrepareAsyncSearch(&context, request_, iCompletionQueue);
    }

    void exec() override
    {
        std::cout << "[MemoSearch] Received response." << std::endl;
        std::cout << " - contains " << reply.memos().size() << " memos." << std::endl;
        for (const auto& memo : reply.memos())
            std::cout << "Memo titled: \"" << memo.title() << "\"" << std::endl;
    }
private:
    model::MemoSearchRq request_;
};

class MemoSearchByIdCall : public BaseCall<model::MemoSvc::Stub, model::MemoSearchRs>
{
public:
    MemoSearchByIdCall(model::MemoSvc::Stub& ioStub, model::IdList iRequest) :
        BaseCall(ioStub), request_(iRequest) {}
    ~MemoSearchByIdCall() = default;

    void registerIn(grpc::CompletionQueue* iCompletionQueue) override
    {
        reader = stub_.PrepareAsyncSearchById(&context, request_, iCompletionQueue);
    }

    void exec() override
    {
        std::cout << "[MemoSearchById] Received response." << std::endl;
        std::cout << " - contains " << reply.memos().size() << " memos." << std::endl;
        for (const auto& memo : reply.memos())
            std::cout << "Memo titled: \"" << memo.title() << "\"" << std::endl;
    }
private:
    model::IdList request_;
};

class MemoCreateCall : public BaseCall<model::MemoSvc::Stub, model::Id>
{
public:
    MemoCreateCall(model::MemoSvc::Stub& ioStub, model::Memo iRequest):
        BaseCall(ioStub), request_(iRequest) {}
    ~MemoCreateCall() = default;

    void registerIn(grpc::CompletionQueue* iCompletionQueue) override
    {
        reader = stub_.PrepareAsyncCreate(&context, request_, iCompletionQueue);
    }

    void exec() override
    {
        std::cout << "[MemoCreate] Received response containing id: ";
        std::cout << reply.value() << std::endl;
    }
private:
    model::Memo request_;
};

class MemoUpdateCall : public BaseCall<model::MemoSvc::Stub, model::OperationStatus>
{
public:
    MemoUpdateCall(model::MemoSvc::Stub& ioStub, model::Memo iRequest) :
        BaseCall(ioStub), request_(iRequest) {}
    ~MemoUpdateCall() = default;

    void registerIn(grpc::CompletionQueue* iCompletionQueue) override
    {
        reader = stub_.PrepareAsyncUpdate(&context, request_, iCompletionQueue);
    }

    void exec() override
    {
        std::cout << "[MemoUpdate] Received response with status: ";
        std::cout << reply.status() << std::endl;
    }
private:
    model::Memo request_;
};

class MemoDeleteCall : public BaseCall<model::MemoSvc::Stub, model::OperationStatus>
{
public:
    MemoDeleteCall(model::MemoSvc::Stub& ioStub, model::Id iRequest) :
        BaseCall(ioStub), request_(iRequest) {}
    ~MemoDeleteCall() = default;

    void registerIn(grpc::CompletionQueue* iCompletionQueue) override
    {
        reader = stub_.PrepareAsyncDelete(&context, request_, iCompletionQueue);
    }

    void exec() override
    {
        std::cout << "[MemoDelete] Received response with status: ";
        std::cout << reply.status() << std::endl;
    }
private:
    model::Id request_;
};

class TagSearchCall : public BaseCall<model::TagSvc::Stub, model::TagSearchRs>
{
public:
    TagSearchCall(model::TagSvc::Stub& ioStub, model::TagSearchRq iRequest) :
        BaseCall(ioStub), request_(iRequest) {}
    ~TagSearchCall() = default;

    void registerIn(grpc::CompletionQueue* iCompletionQueue) override
    {
        reader = stub_.PrepareAsyncSearch(&context, request_, iCompletionQueue);
    }

    void exec() override
    {
        std::cout << "[TagSearch] Received response." << std::endl;
        std::cout << " - contains " << reply.tags().size() << " tags." << std::endl;
        for (const auto& memo : reply.tags())
            std::cout << "Tag named: \"" << memo.name() << "\"" << std::endl;
    }
private:
    model::TagSearchRq request_;
};

class TagCreateCall : public BaseCall<model::TagSvc::Stub, model::OperationStatus>
{
public:
    TagCreateCall(model::TagSvc::Stub& ioStub, model::Tag iRequest):
        BaseCall(ioStub), request_(iRequest) {}
    ~TagCreateCall() = default;

    void registerIn(grpc::CompletionQueue* iCompletionQueue) override
    {
        reader = stub_.PrepareAsyncCreate(&context, request_, iCompletionQueue);
    }

    void exec() override
    {
        std::cout << "[TagCreate] Received response with status: ";
        std::cout << reply.status() << std::endl;
    }
private:
    model::Tag request_;
};

class TagUpdateCall : public BaseCall<model::TagSvc::Stub, model::OperationStatus>
{
public:
    TagUpdateCall(model::TagSvc::Stub& ioStub, model::Tag iRequest) :
        BaseCall(ioStub), request_(iRequest) {}
    ~TagUpdateCall() = default;

    void registerIn(grpc::CompletionQueue* iCompletionQueue) override
    {
        reader = stub_.PrepareAsyncUpdate(&context, request_, iCompletionQueue);
    }

    void exec() override
    {
        std::cout << "[TagUpdate] Received response with status: ";
        std::cout << reply.status() << std::endl;
    }
private:
    model::Tag request_;
};

class TagDeleteCall : public BaseCall<model::TagSvc::Stub, model::OperationStatus>
{
public:
    TagDeleteCall(model::TagSvc::Stub& ioStub, model::TagName iRequest) :
        BaseCall(ioStub), request_(iRequest) {}
    ~TagDeleteCall() = default;

    void registerIn(grpc::CompletionQueue* iCompletionQueue) override
    {
        reader = stub_.PrepareAsyncDelete(&context, request_, iCompletionQueue);
    }

    void exec() override
    {
        std::cout << "[TagDelete] Received response with status: ";
        std::cout << reply.status() << std::endl;
    }
private:
    model::TagName request_;
};


Client::Client(const std::string& iAddress) :
    memoStub_(model::MemoSvc::NewStub(grpc::CreateChannel(
                    iAddress,
                    grpc::InsecureChannelCredentials()))),
    tagStub_(model::TagSvc::NewStub(grpc::CreateChannel(
                    iAddress,
                    grpc::InsecureChannelCredentials())))
    {}

void Client::memoSearch(const std::string& iTitle) {
    model::MemoSearchRq request;
    request.mutable_titleoptions()->set_startswith(iTitle);

    MemoSearchCall* call = new MemoSearchCall(*memoStub_, request);
    call->registerIn(&cq_);
    call->startCall();
}

void Client::memoSearchById(const std::string& iId) {
    model::IdList request;
    request.mutable_ids()->Add()->set_value(iId);

    MemoSearchByIdCall* call = new MemoSearchByIdCall(*memoStub_, request);
    call->registerIn(&cq_);
    call->startCall();
}

void Client::createMemo(const std::string& iTitle)
{
    model::Memo request;
    request.set_title(iTitle);

    MemoCreateCall* call = new MemoCreateCall(*memoStub_, request);
    call->registerIn(&cq_);
    call->startCall();
}

void Client::updateMemo(const std::string& iTitle)
{
    model::Memo request;
    request.set_title(iTitle);

    MemoUpdateCall* call = new MemoUpdateCall(*memoStub_, request);
    call->registerIn(&cq_);
    call->startCall();
}

void Client::deleteMemo(const std::string& iId)
{
    model::Id request;
    request.set_value(iId);

    MemoDeleteCall* call = new MemoDeleteCall(*memoStub_, request);
    call->registerIn(&cq_);
    call->startCall();
}

void Client::tagSearch(const std::string& iTitle) {
    model::TagSearchRq request;
    request.mutable_nameoptions()->set_startswith(iTitle);

    TagSearchCall* call = new TagSearchCall(*tagStub_, request);
    call->registerIn(&cq_);
    call->startCall();
}

void Client::createTag(const std::string& iName)
{
    model::Tag request;
    request.set_name(iName);

    TagCreateCall* call = new TagCreateCall(*tagStub_, request);
    call->registerIn(&cq_);
    call->startCall();
}

void Client::updateTag(const std::string& iName)
{
    model::Tag request;
    request.set_name(iName);

    TagUpdateCall* call = new TagUpdateCall(*tagStub_, request);
    call->registerIn(&cq_);
    call->startCall();
}

void Client::deleteTag(const std::string& iName)
{
    model::TagName request;
    request.set_value(iName);

    TagDeleteCall* call = new TagDeleteCall(*tagStub_, request);
    call->registerIn(&cq_);
    call->startCall();
}

void Client::stop()
{
    cq_.Shutdown();
    void* ignoredTag;
    bool ignoredOk;
    while (cq_.Next(&ignoredTag, &ignoredOk))
    {
        Call* call = static_cast<Call*>(ignoredTag);
        delete call;
    }
}

void Client::AsyncCompleteRpc() {
    void* got_tag;
    bool ok = false;

    // Block until the next result is available in the completion queue "cq".
    while (cq_.Next(&got_tag, &ok)) {
        // The tag in this example is the memory location of the call object
        Call* call = static_cast<Call*>(got_tag);

        // Verify that the request was completed successfully. Note that "ok"
        // corresponds solely to the request for updates introduced by Finish().
        GPR_ASSERT(ok);

        if (call->getStatus().ok())
        {
            call->exec();
        }
        else
            std::cout << "RPC failed" << std::endl;

        // Once we're complete, deallocate the call object.
        delete call;
    }
}

} // namespace client
} // namespace memo
