#pragma once
#include <memory>

namespace memo {
namespace manager {
    class ControllerManager;
    class ViewManager;
} // namespace manager

namespace remote {
    class IMemoDao;
} // namespace remote

using ControllerManagerPtr = std::shared_ptr<manager::ControllerManager>;
using ViewManagerPtr       = std::shared_ptr<manager::ViewManager>;
using MemoDaoPtr           = std::shared_ptr<remote::IMemoDao>;

class IResources
{
public:
    virtual ~IResources() = default;
    virtual const ControllerManagerPtr& controllerManager() = 0;
    virtual const ViewManagerPtr& viewManager() = 0;
    virtual const MemoDaoPtr& memoDao() = 0;
};

class ResourcesImpl : public IResources
{
public:
    static std::shared_ptr<ResourcesImpl> Create(
        const ControllerManagerPtr&,
        const ViewManagerPtr&,
        const MemoDaoPtr&);

    ResourcesImpl(
        const ControllerManagerPtr&,
        const ViewManagerPtr&,
        const MemoDaoPtr&);

    const ControllerManagerPtr& controllerManager() override;
    
    const ViewManagerPtr& viewManager() override;

    const MemoDaoPtr& memoDao() override;

private:
    ControllerManagerPtr controllerManager_;
    ViewManagerPtr viewManager_;
    MemoDaoPtr memoDao_;
};

} // namespace memo
