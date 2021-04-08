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
    view->memoTitleTextEditView()->setKeyFilter(keyFilter_);
    view->memoDescriptionTextEditView()->setKeyFilter(keyFilter_);
    view->memoTagsTextEditView()->setKeyFilter(keyFilter_);
    viewInFocus_ = view->memoTitleTextEditView();
}

MemoCreateController::~MemoCreateController()
{
    keyFilter_->resetView();
}

void MemoCreateController::processInput()
{
    while(run_ && viewInFocus_)
    {
        viewInFocus_->focus();
    }
}

bool MemoCreateController::processKey(int key)
{
    if (!viewInFocus_)
    {
        run_ = false;
        return true;
    }

    auto parentView = view();
    if (key == curses::Key::kTab)
    {
        viewInFocus_->looseFocus();
        if (viewInFocus_ == parentView->memoTitleTextEditView())
        {
            viewInFocus_ = parentView->memoDescriptionTextEditView();
        }
        else if (viewInFocus_ == parentView->memoDescriptionTextEditView())
        {
            viewInFocus_ = parentView->memoTagsTextEditView();
        }
        else
        {
            viewInFocus_ = parentView->memoTitleTextEditView();
        }
        return true;
    }
    else if (key == curses::Key::kShiftTab)
    {
        viewInFocus_->looseFocus();
        if (viewInFocus_ == parentView->memoTitleTextEditView())
        {
            viewInFocus_ = parentView->memoTagsTextEditView();
        }
        else if (viewInFocus_ == parentView->memoDescriptionTextEditView())
        {
            viewInFocus_ = parentView->memoTitleTextEditView();
        }
        else
        {
            viewInFocus_ = parentView->memoDescriptionTextEditView();
        }
        return true;
    }
    else if (key == curses::Key::kEsc)
    {
        viewInFocus_->looseFocus();
        run_ = false;
        getResources()->controllerManager()->pop();
        return true;
    }
    return false;
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

