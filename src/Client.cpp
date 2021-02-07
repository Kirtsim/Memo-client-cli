#include "Client.hpp"
#include "context/Resources.hpp"
#include "view/home/HomeView.hpp"
#include "manager/ViewManager.hpp"
#include "manager/ControllerManager.hpp"
#include "controller/HomeController.hpp"
#include "ncurses/functions.hpp"
#include "remote/MemoDao.hpp"
#include "remote/ListMemoCall.hpp"
#include "remote/factory/MemoCallFactory.hpp"

#include <iostream>

#include <grpcpp/grpcpp.h>

namespace memo {

Client::Client(const std::string& address) :
    memoStub_(model::MemoSvc::NewStub(grpc::CreateChannel(
                    address,
                    grpc::InsecureChannelCredentials()))),
    tagStub_(model::TagSvc::NewStub(grpc::CreateChannel(
                    address,
                    grpc::InsecureChannelCredentials()))),
    viewManager_(std::make_shared<manager::ViewManager>()),
    controllerManager_(std::make_shared<manager::ControllerManager>()),
    resources_(ResourcesImpl::Create(
        controllerManager_,
        viewManager_,
        remote::MemoDaoImpl::Create(std::make_unique<remote::MemoCallFactory>(memoStub_))
    ))
{
    viewManager_->addView(std::make_shared<view::HomeView>(*this, viewManager_));
}

void Client::run()
{
    curses::InitCurses();
    curses::LineBuffering(DISABLE);
    curses::KeyPad(ENABLE);
    curses::PrintTypedInput(DISABLE);
    curses::CursorVisible(DISABLE);
    curses::Refresh();

    auto homeController = std::make_shared<ctrl::HomeController>(resources_);
    controllerManager_->add(homeController);

    while (!controllerManager_->empty())
    {
        auto controller = controllerManager_->getCurrent();
        if (!controller)
        {
            controllerManager_->pop();
            continue;
        }

        auto view = controller->getView();
        if (!view)
            throw std::runtime_error("No view returned by controller.");
        view->refresh();
        controller->processInput();
    }

    curses::CursorVisible(ENABLE);
    curses::CloseCurses();
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
