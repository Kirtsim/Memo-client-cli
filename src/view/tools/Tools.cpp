#include "view/tools/Tools.hpp"
#include "view/IComponent.hpp"
#include "view/View.hpp"

namespace memo::ui::tools {

void CenterComponent(ui::IComponent& component, const Center centerType,
                     const ui::IComponent& parent)
{
    const Bounds& bounds = ComponentBoundaries(parent);
    CenterComponent(component, centerType, bounds);
}

void CenterComponent(ui::IComponent& component, const Center centerType,
                     const Bounds& bounds)
{
    if ((centerType & Center::HORIZONTAL) == Center::HORIZONTAL)
    {
        int newX = (bounds.startX + bounds.endX - component.getWidth()) / 2;
        component.setAbsX(newX);
    }

    if ((centerType & Center::VERTICAL) == Center::VERTICAL)
    {
        int newY = (bounds.startY + bounds.endY - component.getHeight()) / 2;
        component.setAbsY(newY);
    }
}

Bounds ComponentBoundaries(const ui::IComponent& component)
{
    Bounds bounds;
    bounds.startX = component.getAbsX();
    bounds.startY = component.getAbsY();
    bounds.endX = bounds.startX + component.getWidth();
    bounds.endY = bounds.startY + component.getHeight();
    return bounds;
}

void ForceRefresh(const std::shared_ptr<View>& view)
{
    ForceRefresh(view.get());
}

void ForceRefresh(View* view)
{
    if (view)
    {
        view->refreshOnRequest();
        view->refresh();
    }
}

} // namespace memo::ui::tools
