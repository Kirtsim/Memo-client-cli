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
    BaseView(Client& client, const std::shared_ptr<manager::ViewManager>& viewManager);
    virtual ~BaseView();

protected:
    const std::shared_ptr<manager::ViewManager>& getViewManager();
    Client& getClient();
    std::string readInput();
    void print(const std::string& content) const;
    void println(const std::string& content) const;

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
    explicit BaseView(IView* parent=nullptr);
    explicit BaseView(const Size& size, IView* parent=nullptr);
    BaseView(const Size& size, const Position& position, IView* parent=nullptr);

    virtual ~BaseView();
    BaseView(const BaseView&) = delete;
    BaseView(const BaseView&&) = delete;
    BaseView& operator=(const BaseView&) = delete;

    void saveState() override;
    void refresh() override;

    virtual void focus() override;

    void setVisible(bool visible) override;
    bool isVisible() const override;

    void setHeight(int height) override;
    void setWidth(int width) override;
    void setSize(const Size& size) override;
    int getHeight() const override;
    int getWidth() const override;
    Size getSize() const override;

    void setY(int iY) override;
    void setAbsY(int iY) override;
    void setX(int iX) override;
    void setAbsX(int iX) override;
    void setPosition(const Position& pos) override;
    void setAbsPosition(const Position& pos) override;
    int getY() const override;
    int getAbsY() const override;
    int getX() const override;
    int getAbsX() const override;
    Position getPosition() const override;
    Position getAbsPosition() const override;

    void setParentView(IView* parent) override;
    IView* getParentView() override;

    void setBorder(const Border& border) override;
    Border getBorder() const override;

    curses::IWindow& getWindow() override;
    void hideWindow();

protected:
    virtual void beforeViewResized();
    virtual void positionComponents();
    virtual void displayContent();

    void registerSubView(IView::Ptr subView);
    void removeSubView(IView::Ptr subView);
    void displayText(const widget::Text& text);
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
