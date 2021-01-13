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

    curses::IWindow& getWindow() override;
    void hideWindow();

protected:
    virtual void beforeViewResized();
    virtual void positionComponents(curses::IWindow& ioWindow);
    virtual void displayContent(curses::IWindow& ioWindow);

    void registerSubView(IView::Ptr iSubView);
    void removeSubView(IView::Ptr iSubView);
    void displayText(const widget::Text& iText);
    Size getParentSize() const;
    Position getParentPosition() const;

private:
    void applyBorder();

private:
    IView* parentView_;
    Position newPosition_;
    Size newSize_;
    Border newBorder_;

    bool visible_;
    std::shared_ptr<curses::IWindow> window_;
    std::unordered_set<IView::Ptr> subViews_;
};

} // namespace ui
} // namespace memo
