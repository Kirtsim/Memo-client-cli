#include "context/Resources.hpp"

namespace memo {

ResourcesImpl::ResourcesImpl(
    const ControllerManagerPtr& controllerManager,
    const ViewManagerPtr& viewManager,
    const MemoDaoPtr& memoDao)
    : controllerManager_(controllerManager)
    , viewManager_(viewManager)
    , memoDao_(memoDao)
{
}

const ControllerManagerPtr& ResourcesImpl::controllerManager()
{
    return controllerManager_;
}

const ViewManagerPtr& ResourcesImpl::viewManager()
{
    return viewManager_;
}

} // namespace memo
