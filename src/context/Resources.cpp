#include "context/Resources.hpp"
#include "remote/MemoDao.hpp"

namespace memo {

std::shared_ptr<ResourcesImpl> ResourcesImpl::Create(
    const ControllerManagerPtr& controllerManager,
    const ViewManagerPtr& viewManager,
    const MemoDaoPtr& memoDao)
{
    return std::make_shared<ResourcesImpl>(
        controllerManager,
        viewManager,
        memoDao
    );
}

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

const MemoDaoPtr& ResourcesImpl::memoDao()
{
    return memoDao_;
}
} // namespace memo
