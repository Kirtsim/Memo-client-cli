#pragma once
#include "view/BaseView.hpp"

namespace memo {
    class Client;
namespace manager {
    class ViewManager;
} // namespace manager
namespace view {

class MenuView : public BaseView
{
public:
    MenuView(Client& iClient, const std::shared_ptr<manager::ViewManager>& iViewManager);
    MenuView(const MenuView& iOther) = delete;

    ~MenuView();
    void display() override;
private:
    bool processInput(const std::string& iInput);
    void printMenu() const;
    void handleInvalidOption();
};
} // namespace view
} // namespace memo
