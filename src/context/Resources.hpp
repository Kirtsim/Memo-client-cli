#pragma once
#include <memory>

namespace memo {
namespace manager {
    class ControllerManager;
} // namespace manager

namespace remote {
    class IMemoService;
} // namespace remote

using ControllerManagerPtr = std::shared_ptr<manager::ControllerManager>;
using IMemoServicePtr      = std::shared_ptr<remote::IMemoService>;

class IResources
{
public:
    virtual ~IResources() = default;
    virtual const ControllerManagerPtr& controllerManager() = 0;
    virtual const IMemoServicePtr& memoService() = 0;
};

class ResourcesImpl : public IResources
{
public:
    static std::shared_ptr<ResourcesImpl> Create(
        const ControllerManagerPtr&,
        const IMemoServicePtr&);

    ResourcesImpl(
        const ControllerManagerPtr&,
        const IMemoServicePtr&);

    const ControllerManagerPtr& controllerManager() override;
    
    const IMemoServicePtr& memoService() override;

private:
    ControllerManagerPtr controllerManager_;
    IMemoServicePtr memoService_;
};

} // namespace memo
