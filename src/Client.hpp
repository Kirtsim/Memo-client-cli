#pragma once
#include "model/MemoSvc.grpc.pb.h"
#include "model/TagSvc.grpc.pb.h"

#include <grpcpp/impl/codegen/completion_queue.h>
#include <string>

namespace memo {
namespace client {

class Client
{
public:
    explicit Client(const std::string& iAddress);

    void memoSearch(const std::string& iTitle);
    void memoSearchById(const std::string& iId);
    void createMemo(const std::string& iTitle);
    void updateMemo(const std::string& iTitle);
    void deleteMemo(const std::string& iId);
    void tagSearch(const std::string& iTitle);
    void createTag(const std::string& iName);
    void updateTag(const std::string& iName);
    void deleteTag(const std::string& iName);
    void stop();
    void AsyncCompleteRpc();

private:

    std::unique_ptr<model::MemoSvc::Stub> memoStub_;
    std::unique_ptr<model::TagSvc::Stub> tagStub_;
    grpc::CompletionQueue cq_;
};

} // namespace client
} // namespace memo
