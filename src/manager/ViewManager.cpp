#include "manager/ViewManager.hpp"
#include "view/IView.hpp"

namespace memo {
namespace manager {

void ViewManager::addView(const ViewPtr_t& iView)
{
    addView(iView, false);
}

void ViewManager::addView(const ViewPtr_t& iView, const bool iCacheCurrent)
{
    if (!iCacheCurrent && !views_.empty())
    {
        viewsToDestroyLater_.push_back(views_.back());
        popView();
    }
    views_.push_back(iView);
}

ViewManager::ViewPtr_t ViewManager::getCurrentView()
{
    if (views_.empty())
        return nullptr;
    return views_.back();
}

int ViewManager::getViewCount() const
{
    return views_.size();
}

bool ViewManager::popView()
{
    if (views_.empty())
        return false;
    viewsToDestroyLater_.push_back(views_.back());
    views_.pop_back();
    return true;
}

void ViewManager::clearUnusedViews()
{
    viewsToDestroyLater_.clear();
}

} // namespace manager
} // namespace memo
