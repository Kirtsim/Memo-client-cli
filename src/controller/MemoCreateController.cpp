#include "controller/MemoCreateController.hpp"
#include "view/widget/TextView.hpp"
#include "view/widget/TextEditView.hpp"
#include "view/tools/StringTools.hpp"
#include "manager/ControllerManager.hpp"
#include "remote/MemoDao.hpp"

#include "ncurses/keys.hpp"

namespace memo::ctrl {

class TextEditKeyFilter : public ui::KeyFilter
{
public:
    explicit TextEditKeyFilter(MemoCreateController* controller);

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
            saveMemoDetails();
            stop();
            return true;
        }
    }
    return false;
}

bool MemoCreateController::checkMemoTitleAvailability(const std::string& title)
{
    return !title.empty();
}


bool MemoCreateController::saveMemoDetails()
{
    if (!checkMemoTitleAvailability(view()->memoTitle()))
        return false;

    if (auto memoDao = getResources()->memoDao())
    {
        model::Memo memo;
        memo.set_title(view()->memoTitle());
        memo.set_description(view()->memoDescription());

        const auto tagsString = view()->memoTags();
        const auto tagNames = tools::splitText(tagsString, "#");
        for (const auto& tagName : tagNames)
        {
            if (!tagName.empty())
            {
                memo.add_tagnames(tagName);
            }
        }
        const auto response = memoDao->add(memo);
        // TODO: Do something with the ID
        return response.IsInitialized()
            && response.operationstatus().status() == model::OperationStatus::SUCCESS;
    }
    return false;
}

void MemoCreateController::stop()
{
    run_ = false;
    view()->focusSubView(ui::MemoCreateView::kNone);
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

} // namespace memo::ctrl

