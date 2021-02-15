#include "view/tools/Tools.hpp"
#include "view/IComponent.hpp"

namespace memo {
namespace ui {
namespace tools {

void Tools::centerComponent(ui::IComponent& component, const Center centerType,
                            const ui::IComponent& parent)
{
    const Bounds& bounds = componentBoundaries(parent);
    centerComponent(component, centerType, bounds);
}

void Tools::centerComponent(ui::IComponent& component, const Center centerType,
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

Bounds Tools::componentBoundaries(const ui::IComponent& component)
{
    Bounds bounds;
    bounds.startX = component.getAbsX();
    bounds.startY = component.getAbsY();
    bounds.endX = bounds.startX + component.getWidth();
    bounds.endY = bounds.startY + component.getHeight();
    return bounds;
}

} // namespace tools
} // namespace ui
} // namespace memo
