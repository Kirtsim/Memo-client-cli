#include "context/Resources.hpp"

namespace memo {

std::shared_ptr<ResourcesImpl> ResourcesImpl::Create(
        const ControllerManagerPtr& controllerManager,
        const IMemoServicePtr& memoService,
        const ITagServicePtr& tagService)
{
    return std::make_shared<ResourcesImpl>(
        controllerManager,
        memoService,
        tagService
    );
}

ResourcesImpl::ResourcesImpl(
    const ControllerManagerPtr& controllerManager,
    const IMemoServicePtr& memoService,
    const ITagServicePtr& tagService)
: controllerManager_(controllerManager)
    , memoService_(memoService)
    , tagService_(tagService)
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

const ITagServicePtr& ResourcesImpl::tagService()
{
    return tagService_;
}
} // namespace memo
