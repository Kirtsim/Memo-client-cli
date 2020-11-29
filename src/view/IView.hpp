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

#include "view/IComponent.hpp"

struct _win_st;

using Window_t = _win_st;

namespace memo {
namespace ui {

class IView : public IComponent
{
public:
    using Ptr = std::shared_ptr<IView>;

    virtual ~IView() = default;

    virtual void refresh() = 0;
    virtual void saveState() = 0;

    virtual void focus() = 0;

    virtual void setVisible(bool visible) = 0;
    virtual bool isVisible() const = 0;

    virtual void setParentView(IView* iParent) = 0;
    virtual IView* getParentView() = 0;

    virtual void setBorder(const Border& iBorder) = 0;
    virtual Border getBorder() const = 0;

    virtual Window_t& getWindow() = 0;
};

} // namespace ui
} // namespace memo
