#include "context/Resources.hpp"

namespace memo {

std::shared_ptr<ResourcesImpl> ResourcesImpl::Create(
        const ControllerManagerPtr& controllerManager,
        const IMemoServicePtr& memoService)
{
    return std::make_shared<ResourcesImpl>(
        controllerManager,
        memoService
    );
}

ResourcesImpl::ResourcesImpl(
    const ControllerManagerPtr& controllerManager,
    const IMemoServicePtr& memoService)
    : controllerManager_(controllerManager)
    , memoService_(memoService)
{
}

const ControllerManagerPtr& ResourcesImpl::controllerManager()
{
    return controllerManager_;
}

const IMemoServicePtr& ResourcesImpl::memoService()
{
    return memoService_;
}
} // namespace memo
