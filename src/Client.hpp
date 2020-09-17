#pragma once
#include "model/MemoSvc.grpc.pb.h"
#include "model/TagSvc.grpc.pb.h"

#include <string>
#include <memory>

namespace memo {
namespace manager {
    class ViewManager;
} // namespace manager

class Client
{
public:
    explicit Client(const std::string& iAddress);

    /// Retrieves the top View from the ViewManager and displays it.
    /// If theres is no view to be dipslayed the client terminates.
    void run();

    model::MemoSvc::Stub& getMemoStub();
    model::TagSvc::Stub& getTagStub();

private:
    std::unique_ptr<model::MemoSvc::Stub> memoStub_;
    std::unique_ptr<model::TagSvc::Stub>  tagStub_;
    std::shared_ptr<manager::ViewManager> viewManager_;
};

} // namespace memo
