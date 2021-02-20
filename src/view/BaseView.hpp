#pragma once
#include "view/View.hpp"
#include <memory>
#include <unordered_set>

namespace memo {
namespace ui {

class TextView;

class BaseView : public View
{
public:
    explicit BaseView(IComponent* parent=nullptr);
    explicit BaseView(const Size& size, IComponent* parent=nullptr);
    BaseView(const Size& size, const Position& position, IComponent* parent=nullptr);

    virtual ~BaseView();
    BaseView(const BaseView&) = delete;
    BaseView(const BaseView&&) = delete;
    BaseView& operator=(const BaseView&) = delete;

    void saveState() override;
    void refresh() override;
    void refreshOnRequest() override;

    virtual void focus() override;

    void setVisible(bool visible) override;
    bool isVisible() const override;

    void setBorder(const Border& border) override;
    Border getBorder() const override;

    curses::IWindow& getWindow() override;
    void hideWindow();

protected:
    virtual void onSizeChanged(const Size& oldSize, const Size& newSize) override;
    virtual void onPositionChanged(const Position& oldPos, const Position& newPos) override;

    virtual void beforeViewResized();
    virtual void positionComponents();
    virtual void displayContent();
    virtual void printForeground();

    void registerSubView(View::Ptr subView);
    void removeSubView(View::Ptr subView);
    void displayText(const std::string& text, const Position& position);
    Size getParentSize() const;
    Position getParentPosition() const;

    void parentRequestOnRefresh();

private:
    void applyBorder();

private:
    Border newBorder_;

    bool visible_ = true;
    bool needsRefresh_ = true;;
    std::shared_ptr<curses::IWindow> window_;
    std::unordered_set<View::Ptr> subViews_;
};

} // namespace ui
} // namespace memo
