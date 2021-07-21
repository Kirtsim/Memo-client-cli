#pragma once

#include "controller/BaseController.hpp"
#include "view/MemoSearchView.hpp"
#include "manager/ControllerManager.hpp"
#include "model/Memo.hpp"

namespace memo::ctrl {

class SearchController : public BaseController<ui::MemoSearchView>
{
    using ResourcesPtr_t = std::shared_ptr<IResources>;
public:
    explicit SearchController(const ResourcesPtr_t& resources);

    void processInput() override;

private:
    void fetchMemos();

private:
    std::vector<model::Memo> memos_;
};

} // namespace memo::ctrl
