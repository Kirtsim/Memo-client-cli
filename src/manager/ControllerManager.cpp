#include "manager/ControllerManager.hpp"
#include "view/IComponent.hpp"

namespace memo {
namespace manager {

void ControllerManager::replaceLast(const ControllerPtr_t& controller)
{
    pop();
    add(controller);
}

void ControllerManager::add(const ControllerPtr_t& controller)
{
    controllers_.push_back(controller);
}

ControllerManager::ControllerPtr_t ControllerManager::pop()
{
    if (controllers_.empty())
        return nullptr;
    forRemoval_.push_back(controllers_.back());
    controllers_.pop_back();
    return forRemoval_.back();
}

ControllerManager::ControllerPtr_t ControllerManager::getCurrent()
{
    if (controllers_.empty())
        return nullptr;
    return controllers_.back();
}

int ControllerManager::count() const
{
    return controllers_.size();
}

bool ControllerManager::empty() const
{
    return controllers_.empty();
}

void ControllerManager::clearUnused()
{
    forRemoval_.clear();
}

} // namespace manager
} // namespace memo
