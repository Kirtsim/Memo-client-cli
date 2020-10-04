#pragma once
#include <memory>

namespace memo {
namespace manager {
    class ControllerManager;
    class ViewManager;
}

class Resources
{
    using ControllerManagerPtr_t = std::shared_ptr<manager::ControllerManager>;
    using ViewManagerPtr_t       = std::shared_ptr<manager::ViewManager>;

public:
    Resources(const ControllerManagerPtr_t&, const ViewManagerPtr_t&);

    const ControllerManagerPtr_t& getControllerManager();
    const ViewManagerPtr_t& getViewManager();

private:
    ControllerManagerPtr_t controllerManager_;
    ViewManagerPtr_t viewManager_;
};

} // namespace memo
