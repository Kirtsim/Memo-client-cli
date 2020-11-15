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

#include <unordered_set>

struct _win_st;

using WindowPtr_t = Window_t*;

namespace memo {
namespace ui {
namespace widget {
    class Text;
} // namespace widget

class BaseView : public IView
{
public:
    explicit BaseView(IView* iParent=nullptr);
    explicit BaseView(const Size& iSize, IView* iParent=nullptr);
    BaseView(const Size& iSize, const Position& iPosition, IView* iParent=nullptr);

    virtual ~BaseView();
    BaseView(const BaseView&) = delete;
    BaseView(const BaseView&&) = delete;
    BaseView& operator=(const BaseView&) = delete;

    void saveState() override;
    void refresh() override;

    virtual void focus() override;

    void setVisible(bool iVisible) override;
    bool isVisible() const override;

    void setHeight(int iHeight) override;
    void setWidth(int iWidth) override;
    void setSize(const Size& iSize) override;
    int getHeight() const override;
    int getWidth() const override;
    Size getSize() const override;

    void setY(int iY) override;
    void setX(int iX) override;
    void setPosition(const Position& iPos) override;
    int getY() const override;
    int getX() const override;
    Position getPosition() const override;

    void setParentView(IView* iParent) override;
    IView* getParentView() override;

    void setBorder(const Border& iBorder) override;
    Border getBorder() const override;

    Window_t& getWindow() override;

protected:
    virtual void beforeViewResized();
    virtual void positionComponents(Window_t& ioWindow);
    virtual void displayContent(Window_t& ioWindow);

    void registerSubView(IView::Ptr iSubView);
    void removeSubView(IView::Ptr iSubView);
    void displayText(const widget::Text& iText);
    void eraseWindow();
    Size getParentSize() const;
    Position getParentPosition() const;

private:
    void applyBorder();

private:
    IView* parentView_;
    int width_, height_;
    int x_, y_;
    Border border_;

    bool visible_;
    WindowPtr_t window_;
    std::unordered_set<IView::Ptr> subViews_;
};

} // namespace ui
} // namespace memo
