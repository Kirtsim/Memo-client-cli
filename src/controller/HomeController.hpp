#pragma once
#include "controller/BaseController.hpp"

namespace memo {
namespace ui {
    class HomeView;
    class MenuItem;
} // namespace view
namespace ctrl {

class HomeController : public BaseController<ui::HomeView>
{
    using ResourcesPtr_t = std::shared_ptr<IResources>;

public:
    explicit HomeController(const ResourcesPtr_t& resources);
    void processInput() override;
private:
    void onMenuOptionSelected(std::pair<bool, ui::MenuItem> selectedOption);
};

} // namespace ctrl
} // namespace memo
