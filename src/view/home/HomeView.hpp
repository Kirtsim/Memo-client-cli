#pragma once
#include "view/BaseView.hpp"

namespace memo {
    class Client;
namespace manager {
    class ViewManager;
} // namespace manager
namespace view {

class HomeView : public BaseView
{
public:
    HomeView(Client& iClient, const std::shared_ptr<manager::ViewManager>& iViewManager);
    HomeView(const HomeView& iOther) = delete;

    ~HomeView();
    void display() override;
private:
    bool processInput(const std::string& iInput);
    void printMenu() const;
    void handleInvalidOption();
};
} // namespace view
} // namespace memo

#include "view/home/MenuItem.hpp"
#include <vector>

namespace memo {
namespace ui {
namespace widget {
    class Text;
}

class MenuView;

enum E_MenuItem
{
    LIST_MEMOS, LIST_TAGS, CREATE_MEMO, CREATE_TAG, DELETE_MEMO, DELETE_TAG
};

class HomeView : public BaseView
{
public:
    explicit HomeView(IView* iParent=nullptr);
    explicit HomeView(const Size& iSize, IView* iParent=nullptr);
    HomeView(const Size& iSize, const Position& iPosition, IView* iParent=nullptr);
    ~HomeView();

    HomeView(const HomeView&) = delete;
    HomeView(const HomeView&&) = delete;
    HomeView& operator=(const HomeView&) = delete;

    const std::shared_ptr<MenuView>& getMenuView();

    void focus() override;

    void setErrorStatus(const std::string& iStatus);

protected:
    void positionComponents(Window_t& ioWindow) override;
    void displayContent(Window_t& ioWindow) override;

private:
    static const std::vector<MenuItem> kMenuItems;

    std::unique_ptr<widget::Text> errorStatus_;
    std::unique_ptr<widget::Text> windowTitle_;
    std::shared_ptr<MenuView> menuView_;
};

} // namespace ui
} // namespace memo


