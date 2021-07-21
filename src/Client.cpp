#include "Client.hpp"
#include "context/Resources.hpp"
#include "view/home/HomeView.hpp"
#include "manager/ControllerManager.hpp"
#include "controller/HomeController.hpp"
#include "ncurses/functions.hpp"
#include "remote/MemoService.hpp"
#include "remote/TagService.hpp"
#include "remote/factory/MemoCallFactory.hpp"
#include "remote/factory/TagCallFactory.hpp"

#include <grpcpp/grpcpp.h>

namespace memo {

Client::Client(const std::string& address) :
    memoServiceStub_(proto::MemoService::NewStub(grpc::CreateChannel(
            address,
            grpc::InsecureChannelCredentials()))),
    tagServiceStub_(proto::TagService::NewStub(grpc::CreateChannel(
            address,
            grpc::InsecureChannelCredentials()))),
    controllerManager_(std::make_shared<manager::ControllerManager>()),
    resources_(ResourcesImpl::Create(
            controllerManager_,
            remote::MemoServiceImpl::Create(std::make_unique<remote::MemoCallFactory>(memoServiceStub_)),
            remote::TagServiceImpl::Create(std::make_unique<remote::TagCallFactoryImpl>(tagServiceStub_))
    ))
{
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

} // namespace memo
