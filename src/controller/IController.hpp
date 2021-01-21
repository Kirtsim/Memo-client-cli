#pragma once

#include <string>
#include <memory>

namespace memo {
namespace ui {
    class View;
} // namespace view
namespace ctrl {

class IController
{
public:
    virtual ~IController() = default;

    virtual std::shared_ptr<ui::View> getView() = 0;
    virtual void processInput() = 0;
    virtual void saveState() = 0;
};

} // namespace ctrl
} // namespace memo
