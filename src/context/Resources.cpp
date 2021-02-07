#include "context/Resources.hpp"
#include "remote/MemoDao.hpp"

namespace memo {

std::shared_ptr<ResourcesImpl> ResourcesImpl::Create(
    const ControllerManagerPtr& controllerManager,
    const MemoDaoPtr& memoDao)
{
    return std::make_shared<ResourcesImpl>(
        controllerManager,
        memoDao
    );
}

ResourcesImpl::ResourcesImpl(
    const ControllerManagerPtr& controllerManager,
    const MemoDaoPtr& memoDao)
    : controllerManager_(controllerManager)
    , memoDao_(memoDao)
{
}

const ControllerManagerPtr& ResourcesImpl::controllerManager()
{
    return controllerManager_;
}

const MemoDaoPtr& ResourcesImpl::memoDao()
{
    return memoDao_;
}
} // namespace memo
