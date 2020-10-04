#pragma once

#include <string>
#include <memory>

namespace memo {
namespace ui {
    class IView;
} // namespace view
namespace ctrl {

class IController
{
public:
    virtual ~IController() = default;

    virtual std::shared_ptr<ui::IView> getView() = 0;
    virtual void processInput() = 0;
    virtual void saveState() = 0;
};

} // namespace ctrl
} // namespace memo
