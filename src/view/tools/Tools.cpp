#include "view/tools/Tools.hpp"
#include "view/IComponent.hpp"

namespace memo {
namespace ui {
namespace tools {

void Tools::centerComponent(ui::IComponent& ioComponent, const int centerType,
                            const ui::IComponent& iParent)
{
    const Bounds& bounds = componentBoundaries(iParent);
    centerComponent(ioComponent, centerType, bounds);
}

void Tools::centerComponent(ui::IComponent& ioComponent, const int centerType,
                            const Bounds& bounds)
{
    if (centerType & Center::HORIZONTAL)
    {
        int newX = (bounds.startX + bounds.endX - ioComponent.getWidth()) / 2;
        ioComponent.setX(newX);
    }

    if (centerType & Center::VERTICAL)
    {
        int newY = (bounds.startY + bounds.endY - ioComponent.getHeight()) / 2;
        ioComponent.setY(newY);
    }
}

Bounds Tools::componentBoundaries(const ui::IComponent& iComponent)
{
    Bounds bounds;
    bounds.startX = iComponent.getX();
    bounds.startY = iComponent.getY();
    bounds.endX = bounds.startX + iComponent.getWidth();
    bounds.endY = bounds.startY + iComponent.getHeight();
    return bounds;
}

} // namespace tools
} // namespace ui
} // namespace memo
