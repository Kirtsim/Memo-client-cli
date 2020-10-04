#include "context/Resources.hpp"

namespace memo {

Resources::Resources(const ControllerManagerPtr_t& iControllerManager,
                     const ViewManagerPtr_t& iViewManager) :
    controllerManager_(iControllerManager),
    viewManager_(iViewManager)
{}


const Resources::ControllerManagerPtr_t& Resources::getControllerManager()
{
    return controllerManager_;
}

const Resources::ViewManagerPtr_t& Resources::getViewManager()
{
    return viewManager_;
}

} // namespace memo
