#include "controller/MemoCreateController.hpp"
#include "view/widget/TextView.hpp"
#include "view/widget/TextEditView.hpp"
#include "manager/ControllerManager.hpp"

#include "ncurses/functions.hpp"
#include "ncurses/keys.hpp"

namespace memo {
namespace ctrl {

class TextEditKeyFilter : public ui::KeyFilter
{
public:
    TextEditKeyFilter(MemoCreateController* view);

    bool filterKey(int key) override;
    void resetView();

private:
    MemoCreateController* controller_;
};

MemoCreateController::MemoCreateController(const ResourcesPtr_t& resources)
    : BaseController(resources)
    , keyFilter_(std::make_shared<TextEditKeyFilter>(this))
{
    auto view = std::make_shared<ui::MemoCreateView>();
    setView(view);
    view->registerKeyFilter(keyFilter_);
}

MemoCreateController::~MemoCreateController()
{
    keyFilter_->resetView();
}

void MemoCreateController::processInput()
{
    view()->focusSubView(ui::MemoCreateView::kTitleArea);
    while(run_)
    {
        view()->readInput();
    }
}

bool MemoCreateController::processKey(int key)
{
    if (key == curses::Key::kTab)
    {
        view()->focusNextSubView();
        return true;
    }
    else if (key == curses::Key::kShiftTab)
    {
        view()->focusPrevSubView();
        return true;
    }
    else if (key == curses::Key::kEsc)
    {
        stop();
        return true;
    }
    else if (key == curses::Key::kEnter)
    {
        auto viewInFocus = view()->subViewInFocus();
        if (viewInFocus == ui::MemoCreateView::kCancelButton)
        {
            stop();
            return true;
        }
        else if (viewInFocus == ui::MemoCreateView::kConfirmButton)
        {
            stop();
            return true;
        }
    }
    return false;
}

void MemoCreateController::stop()
{
    run_ = false;
    getResources()->controllerManager()->pop();
}

TextEditKeyFilter::TextEditKeyFilter(MemoCreateController* controller)
    : controller_(controller)
{
}

bool TextEditKeyFilter::filterKey(int key)
{
    if (controller_)
    {
        return controller_->processKey(key);
    }
    return false;
}

void TextEditKeyFilter::resetView()
{
    controller_ = nullptr;
}

} // namespace ctrl
} // namespace memo

