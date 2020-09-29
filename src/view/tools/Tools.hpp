#pragma once

namespace memo {
namespace ui {
    class IComponent;
namespace tools {


struct Bounds
{
    int startY, endY;
    int startX, endX;
};

namespace Center {
    const int HORIZONTAL = 1;
    const int VERTICAL = 2;
};

class Tools
{
public:
    Tools() = delete;
    Tools(const Tools&) = delete;
    Tools(const Tools&&) = delete;
    Tools& operator=(const Tools&&) = delete;

    /// Center the given ui component within a parent component
    static void centerComponent(ui::IComponent& ioComponent, int centerType,
                                const ui::IComponent& iParent);

    /// Center the given ui component within the specified bounds
    static void centerComponent(ui::IComponent& ioComponent, int centerType,
                                const Bounds& bounds);

    /// Retrieves the ui component's bounds
    static Bounds componentBoundaries(const ui::IComponent& iComponent);
};

} // namespace tools
} // namespace ui
} // namespace memo
