#pragma once
#include "view/IView.hpp"
#include <memory>

namespace memo {
    class Client;
namespace manager {
    class ViewManager;
}
namespace view {

class BaseView : public IView
{
public:
    BaseView(Client& iClient, const std::shared_ptr<manager::ViewManager>& iViewManager);
    virtual ~BaseView();

protected:
    const std::shared_ptr<manager::ViewManager>& getViewManager();
    Client& getClient();
    std::string readInput();
    void print(const std::string& iContent) const;
    void println(const std::string& iContent) const;

private:
    Client& client_;
    std::shared_ptr<manager::ViewManager> viewManager_;
};

} // namespace view
} // namespace memo

//#include <ncursesw/curses.h>

struct _win_st;

using Window_t = _win_st;
using WindowPtr_t = std::unique_ptr<Window_t>;

namespace memo {
    class Client;
namespace manager {
    class ViewManager;
}
namespace ui {
namespace view {

class BaseView : public IView
{
public:
    explicit BaseView(const IView::Ptr& iParent=nullptr);
    explicit BaseView(const Size& iSize, const IView::Ptr& iParent=nullptr);
    BaseView(const Size& iSize, const Position& iPosition, const IView::Ptr& iParent=nullptr);

    virtual ~BaseView();
    BaseView(const BaseView&) = delete;
    BaseView(const BaseView&&) = delete;
    BaseView& operator=(const BaseView&) = delete;

    void saveState() override;
    void refresh() override;

    void setVisible(bool iVisible) override;
    bool isVisible() const override;

    void setHeight(int iHeight) override;
    void setWidth(int iWidth) override;
    void setSize(const Size& iSize) override;
    int getHeight() const override;
    int getWidth() const override;
    Size getSize() const override;

    void setPosY(int iY) override;
    void setPosX(int iX) override;
    void setPosition(const Position& iPos) override;
    int getPosY() const override;
    int getPosX() const override;
    Position getPosition() const override;

    void setParentView(const IView::Ptr& iParent) override;
    const IView::Ptr& getParentView() override;

    void setBorder(const Border& iBorder) override;
    Border getBorder() const override;

/// *** IView methods not imlemented ***
//  void focus();

protected:
    void eraseWindow();
    Size getParentSize() const;
    Position getParentPosition() const;
    Window_t& getWindow();

private:
    void applyBorder();

private:
    IView::Ptr parentView_;
    int width_, height_;
    int x_, y_;
    Border border_;

    bool visible_;
    WindowPtr_t window_;
};

} // namespace view
} // namespace ui
} // namespace memo
