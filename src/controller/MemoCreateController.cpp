#include "controller/MemoCreateController.hpp"
#include "view/widget/TextEditView.hpp"
#include "view/tools/StringTools.hpp"
#include "manager/ControllerManager.hpp"
#include "remote/MemoDao.hpp"
#include "remote/MemoService.hpp"
#include "remote/AddMemoRequest.hpp"
#include "remote/ServiceResponse.hpp"
#include "model/Tag.hpp"
#include "model/ModelDefs.hpp"

#include "ncurses/keys.hpp"

#include <ctime>

namespace memo::ctrl {
#ifndef FEATURE_DEVEL
#define FEATURE_DEVEL true
#endif

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
#if FEATURE_DEVEL
    auto memo = std::make_shared<model::Memo>();
    memo->setTitle(view()->memoTitle());
    memo->setDescription(view()->memoDescription());
    memo->setTimestamp(static_cast<unsigned long>(std::time(nullptr)));
    // TODO: also set the tags.
    remote::AddMemoRequestBuilder requestBuilder;
    requestBuilder.setRequestUuid("abcd-efgh-ijkl-mnop")
                  .setMemo(memo);
    auto memoService = getResources()->memoService();
    if (!memoService) return false;

    auto response = memoService->addMemo(requestBuilder.build());
    if (!response || response->status() != remote::ResponseStatus::kSuccess)
    {
        // TODO: handle this scenario. Log + display a message to the user.
        return false;
    }
    // TODO: update current memo. Add it to the list of memos held in memory. Inform the user
    // about the success.
    return true; //response->status() == remote::ResponseStatus::kSuccess;
#else
    if (auto memoDao = getResources()->memoDao())
    {
        proto::Memo memo;
        memo.set_title(view()->memoTitle());
        memo.set_description(view()->memoDescription());

        const auto tagsString = view()->memoTags();
        const auto tagNames = tools::splitText(tagsString, "#");
        const auto tags = fetchTags(tagNames);
        for (const auto& tag : tags)
        {
            if (tag)
                memo.add_tag_ids(tag->id());
        }
        const auto response = memoDao->add(memo);
        // TODO: Do something with the ID
        return response.IsInitialized()
            && response.operation_status().type() == proto::OperationStatus::SUCCESS;
    }
#endif
    return false;
}

std::vector<model::TagPtr> MemoCreateController::fetchTags(const std::vector<std::string>& /*tagNames*/) const
{
    // TODO:
    //auto tagDao = getResources()->TagDao();
    return {};
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

