#pragma once
#include "controller/BaseController.hpp"
#include "view/MemoCreateView.hpp"

namespace memo::ui {
    class TextEditView;
}
namespace memo::ctrl {

class TextEditKeyFilter;


class MemoCreateController : public BaseController<ui::MemoCreateView>
{
    using ResourcesPtr_t = std::shared_ptr<IResources>;

public:
    explicit MemoCreateController(const ResourcesPtr_t& resources);
    ~MemoCreateController() override;

    void processInput() override;
    bool checkMemoTitleAvailability(const std::string& title);
    bool saveMemoDetails();

private:
    bool processKey(int key);
    void stop();

private:
    friend class TextEditKeyFilter;
    std::shared_ptr<TextEditKeyFilter> keyFilter_;
    bool run_ = true;
};

} // namespace memo::ctrl
