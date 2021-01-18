#include "context/Resources.hpp"

namespace memo {

Resources::Resources(const ControllerManagerPtr_t& controllerManager,
                     const ViewManagerPtr_t& viewManager) :
    controllerManager_(controllerManager),
    viewManager_(viewManager)
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
