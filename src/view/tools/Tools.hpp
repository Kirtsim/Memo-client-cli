#pragma once
#include "utils/Enums.hpp"

namespace memo::ui { class IComponent; }
namespace memo::ui::tools {

struct Bounds
{
    int startY, endY;
    int startX, endX;
};

/// Center the given ui component within a parent component
void CenterComponent(ui::IComponent& component, Center centerType,
                            const ui::IComponent& parent);

/// Center the given ui component within the specified bounds
void CenterComponent(ui::IComponent& component, Center centerType,
                            const Bounds& bounds);

/// Retrieves the ui component's bounds
Bounds ComponentBoundaries(const ui::IComponent& component);

} // namespace memo::ui::tools
