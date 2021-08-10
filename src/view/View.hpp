#pragma once
#include "view/BaseComponent.hpp"
#include <memory>
#include <functional>

namespace memo {
namespace curses {
    class IWindow;
} // namespace curses
namespace ui {

class View : public BaseComponent
{
public:
    using Ptr = std::shared_ptr<View>;
    explicit View(IComponent* parent=nullptr);
    explicit View(const Size& size, IComponent* parent=nullptr);
    View(const Size& size, const Position& position, IComponent* parent=nullptr);

    ~View() override = default;

    virtual void refresh() = 0;
    virtual void saveState() = 0;
    virtual void refreshOnRequest() = 0;

    virtual void focus() = 0;
    virtual void looseFocus() = 0;
    virtual bool hasFocus() const = 0;

    virtual void readInput() = 0;

    virtual void setKeyFilter(const std::function<bool(int)>& filterFunction) = 0;

    virtual void setVisible(bool visible) = 0;
    virtual bool isVisible() const = 0;

    virtual void setBorder(const Border& border) = 0;
    virtual Border getBorder() const = 0;

    virtual curses::IWindow& getWindow() = 0;
};

} // namespace ui
} // namespace memo
