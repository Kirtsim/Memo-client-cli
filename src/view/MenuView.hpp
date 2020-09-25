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

namespace memo {
namespace ui {

class MenuView : public BaseView
{
public:
    explicit MenuView(const IView::Ptr& iParent=nullptr);
    explicit MenuView(const Size& iSize, const IView::Ptr& iParent=nullptr);
    MenuView(const Size& iSize, const Position& iPosition, const IView::Ptr& iParent=nullptr);
    ~MenuView();

    MenuView(const MenuView&) = delete;
    MenuView(const MenuView&&) = delete;
    MenuView& operator=(const MenuView&) = delete;

    void focus() override;

protected:
    void populateWindow(Window_t& ioWindow) override;
};

} // namespace ui
} // namespace memo


