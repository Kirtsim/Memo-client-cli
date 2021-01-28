#pragma once
#include "controller/IController.hpp"
#include "context/Resources.hpp"

namespace memo {
namespace ui {
    class View;
} // namespace view
namespace ctrl {

template <class ViewType>
class BaseController : public IController
{
    using ViewPtr_t = std::shared_ptr<ViewType>;
    using ResourcesPtr_t = std::shared_ptr<IResources>;

public:
    BaseController(const ResourcesPtr_t& resources);
    virtual ~BaseController() = default;

    std::shared_ptr<ui::View> getView() override;
    virtual void saveState() override;

protected:
    void setView(const ViewPtr_t& view);
    const ViewPtr_t& view();
    const ResourcesPtr_t& getResources();

private:
    ResourcesPtr_t resources_;
    ViewPtr_t view_;
};

template <class ViewType>
BaseController<ViewType>::BaseController(const ResourcesPtr_t& resources) :
    resources_(resources)
{}

template <class ViewType>
void BaseController<ViewType>::setView(const ViewPtr_t& view)
{
    view_ = view;
}

template <class ViewType>
const std::shared_ptr<ViewType>& BaseController<ViewType>::view()
{
    return view_;
}

template <class ViewType>
const std::shared_ptr<IResources>& BaseController<ViewType>::getResources()
{
    return resources_;
}

template <class ViewType>
std::shared_ptr<ui::View> BaseController<ViewType>::getView()
{
    return view_;
}

template <class ViewType>
void BaseController<ViewType>::saveState() {}

} // namespace ctrl
} // namespace memo
