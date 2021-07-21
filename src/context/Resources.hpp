#pragma once
#include <memory>

namespace memo {
namespace manager {
    class ControllerManager;
} // namespace manager

namespace remote {
    class IMemoService;
    class ITagService;
} // namespace remote

using ControllerManagerPtr = std::shared_ptr<manager::ControllerManager>;
using IMemoServicePtr      = std::shared_ptr<remote::IMemoService>;
using ITagServicePtr       = std::shared_ptr<remote::ITagService>;

class IResources
{
public:
    virtual ~IResources() = default;
    virtual const ControllerManagerPtr& controllerManager() = 0;
    virtual const IMemoServicePtr& memoService() = 0;
    virtual const ITagServicePtr& tagService() = 0;
};

class ResourcesImpl : public IResources
{
public:
    static std::shared_ptr<ResourcesImpl> Create(
        const ControllerManagerPtr&,
        const IMemoServicePtr&,
        const ITagServicePtr&);

    ResourcesImpl(
        const ControllerManagerPtr&,
        const IMemoServicePtr&,
        const ITagServicePtr&);

    const ControllerManagerPtr& controllerManager() override;
    
    const IMemoServicePtr& memoService() override;

    const ITagServicePtr& tagService() override;

private:
    ControllerManagerPtr controllerManager_;
    IMemoServicePtr memoService_;
    ITagServicePtr tagService_;
};

} // namespace memo
