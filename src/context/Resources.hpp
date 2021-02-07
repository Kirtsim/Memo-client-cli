#pragma once
#include <memory>

namespace memo {
namespace manager {
    class ControllerManager;
} // namespace manager

namespace remote {
    class IMemoDao;
} // namespace remote

using ControllerManagerPtr = std::shared_ptr<manager::ControllerManager>;
using MemoDaoPtr           = std::shared_ptr<remote::IMemoDao>;

class IResources
{
public:
    virtual ~IResources() = default;
    virtual const ControllerManagerPtr& controllerManager() = 0;
    virtual const MemoDaoPtr& memoDao() = 0;
};

class ResourcesImpl : public IResources
{
public:
    static std::shared_ptr<ResourcesImpl> Create(
        const ControllerManagerPtr&,
        const MemoDaoPtr&);

    ResourcesImpl(
        const ControllerManagerPtr&,
        const MemoDaoPtr&);

    const ControllerManagerPtr& controllerManager() override;
    
    const MemoDaoPtr& memoDao() override;

private:
    ControllerManagerPtr controllerManager_;
    MemoDaoPtr memoDao_;
};

} // namespace memo
