#pragma once
#include "controller/BaseController.hpp"
#include "view/MemoCreateView.hpp"

namespace memo {
namespace ui {
    class TextEditView;
}
namespace ctrl {

class TextEditKeyFilter;


class MemoCreateController : public BaseController<ui::MemoCreateView>
{
    using ResourcesPtr_t = std::shared_ptr<IResources>;

public:
    MemoCreateController(const ResourcesPtr_t& resources);
    ~MemoCreateController();

    void processInput() override;

private:
    bool processKey(int key);
    bool stop();

private:
    friend class TextEditKeyFilter;
    std::shared_ptr<TextEditKeyFilter> keyFilter_;
    bool run_ = true;
};

} // namespace ctrl
} // namespace memo
