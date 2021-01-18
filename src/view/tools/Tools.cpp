#include "view/tools/Tools.hpp"
#include "view/IComponent.hpp"

namespace memo {
namespace ui {
namespace tools {

void Tools::centerComponent(ui::IComponent& component, const int centerType,
                            const ui::IComponent& parent)
{
    const Bounds& bounds = componentBoundaries(parent);
    centerComponent(component, centerType, bounds);
}

void Tools::centerComponent(ui::IComponent& component, const int centerType,
                            const Bounds& bounds)
{
    if (centerType & Center::HORIZONTAL)
    {
        int newX = (bounds.startX + bounds.endX - component.getWidth()) / 2;
        component.setX(newX);
    }

    if (centerType & Center::VERTICAL)
    {
        int newY = (bounds.startY + bounds.endY - component.getHeight()) / 2;
        component.setY(newY);
    }
}

Bounds Tools::componentBoundaries(const ui::IComponent& component)
{
    Bounds bounds;
    bounds.startX = component.getX();
    bounds.startY = component.getY();
    bounds.endX = bounds.startX + component.getWidth();
    bounds.endY = bounds.startY + component.getHeight();
    return bounds;
}

} // namespace tools
} // namespace ui
} // namespace memo
