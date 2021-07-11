#pragma once
#include <memory>

namespace memo {
namespace manager {
    class ControllerManager;
} // namespace manager

namespace remote {
    class IMemoDao;
    class IMemoService;
} // namespace remote

using ControllerManagerPtr = std::shared_ptr<manager::ControllerManager>;
using MemoDaoPtr           = std::shared_ptr<remote::IMemoDao>;
using IMemoServicePtr      = std::shared_ptr<remote::IMemoService>;

class IResources
{
public:
    virtual ~IResources() = default;
    virtual const ControllerManagerPtr& controllerManager() = 0;
    virtual const MemoDaoPtr& memoDao() = 0;
    virtual const IMemoServicePtr& memoService() = 0;
};

class ResourcesImpl : public IResources
{
public:
    static std::shared_ptr<ResourcesImpl> Create(
        const ControllerManagerPtr&,
        const MemoDaoPtr&,
        const IMemoServicePtr&);

    ResourcesImpl(
        const ControllerManagerPtr&,
        const MemoDaoPtr&,
        const IMemoServicePtr&);

    const ControllerManagerPtr& controllerManager() override;
    
    const MemoDaoPtr& memoDao() override;

    const IMemoServicePtr& memoService() override;

private:
    ControllerManagerPtr controllerManager_;
    MemoDaoPtr memoDao_;
    IMemoServicePtr memoService_;
};

} // namespace memo
