#pragma once
#include "utils/Enums.hpp"

namespace memo {
namespace ui {
    class IComponent;
namespace tools {


struct Bounds
{
    int startY, endY;
    int startX, endX;
};

class Tools
{
public:
    Tools() = delete;
    Tools(const Tools&) = delete;
    Tools(const Tools&&) = delete;
    Tools& operator=(const Tools&&) = delete;

    /// Center the given ui component within a parent component
    static void centerComponent(ui::IComponent& component, Center centerType,
                                const ui::IComponent& parent);

    /// Center the given ui component within the specified bounds
    static void centerComponent(ui::IComponent& component, Center centerType,
                                const Bounds& bounds);

    /// Retrieves the ui component's bounds
    static Bounds componentBoundaries(const ui::IComponent& component);
};

} // namespace tools
} // namespace ui
} // namespace memo
