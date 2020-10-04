#pragma once
#include "controller/BaseController.hpp"

namespace memo {
namespace ui {
    class HomeView;
} // namespace view
namespace ctrl {

class HomeController : public BaseController<ui::HomeView>
{
    using ResourcesPtr_t = std::shared_ptr<Resources>;

public:
    HomeController(const ResourcesPtr_t& iResources);
    void processInput() override;
private:
    void onMenuOptionSelected(int iSelectedOption);
};

} // namespace ctrl
} // namespace memo
