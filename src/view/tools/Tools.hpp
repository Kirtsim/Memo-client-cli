#pragma once
#include "utils/Enums.hpp"
#include <memory>

namespace memo::ui { class IComponent; }
namespace memo::ui { class View; }
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

/// @brief Makes the view refresh immediately.
/// @param view View shared pointer that is to be refreshed.
void ForceRefresh(const std::shared_ptr<View>& view);

/// @brief Makes the view refresh immediately.
/// @param view View shared pointer that is to be refreshed.
void ForceRefresh(View* view);

} // namespace memo::ui::tools
