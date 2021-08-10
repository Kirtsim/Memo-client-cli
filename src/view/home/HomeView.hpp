#pragma once
#include "view/BaseView.hpp"
#include "view/home/MenuItem.hpp"
#include <vector>

namespace memo::ui {
    class TextView;
    class MenuView;

enum E_MenuItem
{
    LIST_MEMOS, LIST_TAGS, CREATE_MEMO, CREATE_TAG, DELETE_MEMO, DELETE_TAG, EXIT
};

class HomeView : public BaseView
{
public:
    explicit HomeView(IComponent* parent=nullptr);
    explicit HomeView(const Size& size, IComponent* parent=nullptr);
    HomeView(const Size& size, const Position& position, IComponent* parent=nullptr);
    ~HomeView() override;

    HomeView(const HomeView&) = delete;
    HomeView(const HomeView&&) = delete;
    HomeView& operator=(const HomeView&) = delete;

    const std::shared_ptr<MenuView>& getMenuView();

    void setErrorStatus(const std::string& status);

protected:
    void displayContent() override;

private:
    static const std::vector<MenuItem> kMenuItems;

    std::shared_ptr<TextView> errorStatus_;
    std::shared_ptr<TextView> windowTitle_;
    std::shared_ptr<MenuView> menuView_;
};

} // namespace memo::ui
