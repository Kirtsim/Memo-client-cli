#pragma once
#include "MemoSvc.grpc.pb.h"
#include "TagSvc.grpc.pb.h"

#include <string>
#include <memory>

namespace memo {
namespace manager {
    class ControllerManager;
} // namespace manager

class IResources;

class Client
{
public:
    explicit Client(const std::string& address);

    /// Retrieves the top View from the ViewManager and displays it.
    /// If theres is no view to be dipslayed the client terminates.
    void run();

    proto::MemoSvc::Stub& getMemoStub();
    proto::TagSvc::Stub& getTagStub();

private:
    std::shared_ptr<proto::MemoSvc::Stub> memoStub_;
    std::shared_ptr<proto::TagSvc::Stub>  tagStub_;
    std::shared_ptr<manager::ControllerManager> controllerManager_;
    std::shared_ptr<IResources> resources_;
};

} // namespace memo
