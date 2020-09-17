#include "Client.hpp"
#include "view/MenuView.hpp"
#include "manager/ViewManager.hpp"

#include <iostream>

#include <grpcpp/grpcpp.h>

namespace memo {

Client::Client(const std::string& iAddress) :
    memoStub_(model::MemoSvc::NewStub(grpc::CreateChannel(
                    iAddress,
                    grpc::InsecureChannelCredentials()))),
    tagStub_(model::TagSvc::NewStub(grpc::CreateChannel(
                    iAddress,
                    grpc::InsecureChannelCredentials()))),
    viewManager_(new manager::ViewManager)
{
    viewManager_->addView(std::make_shared<view::MenuView>(*this, viewManager_));
}

void Client::run()
{
    while (viewManager_->getViewCount() > 0)
    {
        viewManager_->clearUnusedViews();
        auto view = viewManager_->getCurrentView();
        view->display();
    }
    std::cout << "Closing client.\n";
}

model::MemoSvc::Stub& Client::getMemoStub()
{
    return *memoStub_;
}

model::TagSvc::Stub& Client::getTagStub()
{
    return *tagStub_;
}

} // namespace memo
