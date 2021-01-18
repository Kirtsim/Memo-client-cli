#pragma once
#include <list>
#include <memory>

namespace memo {
namespace view {
    class IView;
}
namespace manager {

class ViewManager
{
    using ViewPtr_t = std::shared_ptr<view::IView>;
public:
    void addView(const ViewPtr_t& view);
    void addView(const ViewPtr_t& view, bool cacheCurrent);

    ViewPtr_t getCurrentView();
    int getViewCount() const;
    bool popView();

    /// Previous Views are not destroyed immediately after a new View is added,
    /// given that they still may have ongoing work to finish. Instead, they
    /// are cached and can be removed by calling this function.
    void clearUnusedViews();

private:
    std::list<ViewPtr_t> views_;
    std::list<ViewPtr_t> viewsToDestroyLater_;
};
} // namespace manager
} // namespace memo
