#pragma once
#include <memory>

namespace memo {
namespace view {

class IView
{
public:
    virtual ~IView() = default;

    virtual void display() = 0;
};
} // namespace view
} // namespace memo

#include "view/BaseComponent.hpp"

namespace memo {
namespace curses {
    class IWindow;
} // namespace curses
namespace ui {

class IView : public BaseComponent
{
public:
    using Ptr = std::shared_ptr<IView>;
    explicit IView(IComponent* parent=nullptr);
    explicit IView(const Size& size, IComponent* parent=nullptr);
    IView(const Size& size, const Position& position, IComponent* parent=nullptr);

    virtual ~IView() = default;

    virtual void refresh() = 0;
    virtual void saveState() = 0;

    virtual void focus() = 0;

    virtual void setVisible(bool visible) = 0;
    virtual bool isVisible() const = 0;

    virtual void setBorder(const Border& border) = 0;
    virtual Border getBorder() const = 0;

    virtual curses::IWindow& getWindow() = 0;
};

} // namespace ui
} // namespace memo
