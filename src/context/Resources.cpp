#include "context/Resources.hpp"
#include "remote/MemoDao.hpp"

namespace memo {

std::shared_ptr<ResourcesImpl> ResourcesImpl::Create(
        const ControllerManagerPtr& controllerManager,
        const MemoDaoPtr& memoDao,
        const IMemoServicePtr& memoService)
{
    return std::make_shared<ResourcesImpl>(
        controllerManager,
        memoDao,
        memoService
    );
}

ResourcesImpl::ResourcesImpl(
    const ControllerManagerPtr& controllerManager,
    const MemoDaoPtr& memoDao,
    const IMemoServicePtr& memoService)
    : controllerManager_(controllerManager)
    , memoDao_(memoDao)
    , memoService_(memoService)
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

const IMemoServicePtr& ResourcesImpl::memoService()
{
    return memoService_;
}
} // namespace memo
