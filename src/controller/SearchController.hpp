#pragma once

#include "controller/BaseController.hpp"
//#include "view/widget/ListView.hpp"
#include "view/MemoSearchView.hpp"
#include "manager/ControllerManager.hpp"
#include "model/Memo.pb.h"

namespace memo {

namespace model {

} // namespace model
namespace ui {
    class ListView;
} // namespace ui

namespace ctrl {

class SearchController : public BaseController<ui::MemoSearchView>
{
    using ResourcesPtr_t = std::shared_ptr<IResources>;
public:
    SearchController(const ResourcesPtr_t& resources);
    void processInput() override;

private:
    std::vector<model::Memo> memos_;
};

} // namespace ctrl
} // namespace memo
