#pragma once
#include "controller/BaseController.hpp"
#include "view/MemoCreateView.hpp"

namespace memo {
namespace ctrl {

class MemoCreateController : public BaseController<ui::MemoCreateView>
{
    using ResourcesPtr_t = std::shared_ptr<IResources>;

public:
    MemoCreateController(const ResourcesPtr_t& resources);

    void processInput() override;
};

} // namespace ctrl
} // namespace memo
