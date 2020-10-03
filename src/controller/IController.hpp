#pragma once

#include <string>
#include <memory>

namespace memo {
namespace view {
    class IView;
} // namespace view
namespace ctrl {

class IController
{
public:
    virtual std::shared_ptr<view::IView> getView() = 0;
    virtual void processInput() = 0;
    virtual void saveState() = 0;
    virtual const std::string& getId() const = 0;
};

} // namespace ctrl
} // namespace memo
