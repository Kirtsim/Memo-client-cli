#include "Client.hpp"
#include "context/Resources.hpp"
#include "view/home/HomeView.hpp"
#include "manager/ViewManager.hpp"
#include "manager/ControllerManager.hpp"
#include "controller/IController.hpp"

#include <iostream>

#include <grpcpp/grpcpp.h>
#include <ncursesw/curses.h>

namespace memo {

Client::Client(const std::string& iAddress) :
    memoStub_(model::MemoSvc::NewStub(grpc::CreateChannel(
                    iAddress,
                    grpc::InsecureChannelCredentials()))),
    tagStub_(model::TagSvc::NewStub(grpc::CreateChannel(
                    iAddress,
                    grpc::InsecureChannelCredentials()))),
    viewManager_(new manager::ViewManager),
    controllerManager_(new manager::ControllerManager),
    resources_(new Resources(controllerManager_, viewManager_))
{
    viewManager_->addView(std::make_shared<view::HomeView>(*this, viewManager_));
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

void Client::runcurses()
{
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    refresh();
    curs_set(0);

    ui::HomeView home;
    home.refresh();
    home.focus();

    while(getch() != 'q');

    curs_set(1);
    endwin();
}

void Client::runcontroller()
{
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    refresh();
    curs_set(0);

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

    while(getch() != 'q');

    curs_set(1);
    endwin();
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
