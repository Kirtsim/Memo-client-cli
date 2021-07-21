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

private:
    std::shared_ptr<proto::MemoService::Stub> memoServiceStub_;
    std::shared_ptr<proto::TagService::Stub> tagServiceStub_;
    std::shared_ptr<manager::ControllerManager> controllerManager_;
    std::shared_ptr<IResources> resources_;
};

} // namespace memo
