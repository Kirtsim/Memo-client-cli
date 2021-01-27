#pragma once

#include "controller/BaseController.hpp"
#include "view/widget/ListView.hpp"
#include "manager/ControllerManager.hpp"

namespace memo {
namespace ui {
    class ListView;
} // namespace ui
namespace ctrl {

class SearchController : public BaseController<ui::ListView>
{
    using ResourcesPtr_t = std::shared_ptr<Resources>;
public:
    SearchController(const ResourcesPtr_t& resources);
    void processInput() override;
private:
    
};

} // namespace ctrl
} // namespace memo
