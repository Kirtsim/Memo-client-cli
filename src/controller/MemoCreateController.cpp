#include "controller/MemoCreateController.hpp"
#include "view/widget/TextEditView.hpp"
#include "view/widget/TagPickerView.hpp"
#include "view/dialog/MessageDialog.hpp"
#include "view/tools/StringTools.hpp"
#include "view/tools/Tools.hpp"
#include "manager/ControllerManager.hpp"
#include "remote/MemoService.hpp"
#include "remote/AddMemoRequest.hpp"
#include "remote/ServiceResponse.hpp"
#include "model/Memo.hpp"
#include "model/ModelDefs.hpp"
#include "remote/TagService.hpp"
#include "remote/ListTagsRequest.hpp"
#include "remote/ListTagsResponseData.hpp"

#include "ncurses/keys.hpp"

#include <ctime>

namespace memo::ctrl {

namespace {
    std::vector<std::string> extractTagNames(const std::vector<model::TagPtr>& tags);
    std::vector<std::string> extractTagNamesThatStartWithQuery(const std::vector<model::TagPtr>& tags,
                                                               const std::string& query);
} // namespace

MemoCreateController::MemoCreateController(const ResourcesPtr_t& resources)
    : BaseController(resources)
{
    auto view = std::make_shared<ui::MemoCreateView>();
    setView(view);
    view->setKeyFilter([&](int key) { return processKey(key); });
    view->doOnCancelButtonClicked([&]() { stop(); });
    view->doOnConfirmButtonClicked([&]() {
        saveMemoDetails();
        stop();
    });

    tags_ = fetchTags();
}

MemoCreateController::~MemoCreateController() = default;

void MemoCreateController::processInput()
{
    view()->focusSubView(ui::MemoCreateView::kTitleArea);
    view()->readInput();
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
    else if (key == curses::Key::kEnter && view()->subViewInFocus() == ui::MemoCreateView::kTagsArea)
    {
        pickTags();
        return true;
    }
    else if (key == curses::Key::kEsc)
    {
        stop();
        return true;
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
    auto memo = std::make_shared<model::Memo>();
    memo->setTitle(view()->memoTitle());
    memo->setDescription(view()->memoDescription());
    memo->setTimestamp(static_cast<unsigned long>(std::time(nullptr)));
    memo->setTags(selectedTags_);
    remote::AddMemoRequestBuilder requestBuilder;
    requestBuilder.setRequestUuid("abcd-efgh-ijkl-mnop")
                  .setMemo(memo);
    auto memoService = getResources()->memoService();
    if (!memoService) return false;

    auto response = memoService->addMemo(requestBuilder.build());
    if (!response || response->status() != remote::ResponseStatus::kSuccess)
    {
        // TODO: Log a message.
        ui::MessageDialog::Display("Memo could not be created.", getView().get());
        return false;
    }

    ui::MessageDialog::Display("Memo created successfully.", getView().get());
    // TODO: update current memo. Add it to the list of memos held in memory. Inform the user
    // about the success.
    return true; //response->status() == remote::ResponseStatus::kSuccess;
}

std::vector<model::TagPtr> MemoCreateController::fetchTags() const
{
    auto service = getResources()->tagService();
    if (!service)
    {
        // TODO: Log a message.
        return {};
    }
    remote::ListTagsRequestBuilder requestBuilder;
    requestBuilder.setUuid("memo-create-controller-fetch-tags") // TODO: generate a real uuid.
                  .setView(remote::ModelView::kMinimalView);
    auto response = service->listTags(requestBuilder.build());
    if (!response || response->status() == remote::ResponseStatus::kError)
    {
        // TODO: Log a message.
        return {};
    }

    const auto& responseData = response->data();
    return responseData.tags();
}

void MemoCreateController::stop()
{
    run_ = false;
    view()->focusSubView(ui::MemoCreateView::kNone);
    view()->looseFocus();
    getResources()->controllerManager()->pop();
}

void MemoCreateController::pickTags()
{
    auto tagPicker = std::make_shared<ui::TagPickerView>(getView().get());
    tagPicker->setWidth(static_cast<int>(view()->getWidth() * 0.3));
    tagPicker->setHeight(static_cast<int>(view()->getHeight() * 0.7));
    ui::tools::Tools::centerComponent(*tagPicker, Center::CENTER, *view());

    std::vector<model::TagPtr> tagSelection = selectedTags_;
    const auto allTagNames = extractTagNames(tags_);
    const auto allSelectedTagNames = extractTagNames((tagSelection));
    tagPicker->displayTags(allTagNames);
    tagPicker->displaySelectedTagNames(allSelectedTagNames);
    tagPicker->setSearchBarChangedCallback([&](const std::string& query)
    {
        onTagSearchQueryChanged(query, tagPicker, tagSelection);
    });

    tagPicker->setTagSelectionChangedCallback([&](const std::string& tagName, bool selected)
    {
        onTagSelectionChanged(tagName, selected, tagPicker, tagSelection);
    });

    const auto tagsSelected = tagPicker->display();
    if (tagsSelected)
    {
        selectedTags_ = tagSelection;
        view()->displayTagNames(extractTagNames(selectedTags_));
    }
    view()->refreshOnRequest();
    view()->refresh();
}

void MemoCreateController::onTagSearchQueryChanged(const std::string& query, const TagPickerViewPtr& tagPicker,
                                                   const std::vector<model::TagPtr>& selectedTags)
{
    if (!tagPicker)
        return;

    const auto queriedTagNames = extractTagNamesThatStartWithQuery(tags_, query);
    const auto selectedTagNames = extractTagNamesThatStartWithQuery(selectedTags, query);
    tagPicker->displayTags(queriedTagNames);
    tagPicker->displaySelectedTagNames(selectedTagNames);
    tagPicker->refresh();
}

void MemoCreateController::onTagSelectionChanged(const std::string& tagName, bool selected,
                                                 const TagPickerViewPtr& tagPicker,
                                                 std::vector<model::TagPtr>& selectedTags)
{
    if (!tagPicker)
        return;
    if (selected)
    {
        auto selectedIter = std::find_if(selectedTags.begin(), selectedTags.end(),
                                         [&tagName](const model::TagPtr& tag)
                                         { return tag && tag->name() == tagName; });
        const bool tagNotSelected = (selectedIter == std::end(selectedTags));
        if (tagNotSelected)
        {
            auto tagIter = std::find_if(tags_.begin(), tags_.end(),
                                        [&tagName](const model::TagPtr& tag)
                                        { return tag && tag->name() == tagName; });
            selectedTags.emplace_back(*tagIter);
        }
    }
    else
    {
        auto iter = std::remove_if(selectedTags.begin(), selectedTags.end(),
                                   [&tagName](const model::TagPtr& tag)
                                   { return !tag || tag->name() == tagName; });
        selectedTags.erase(iter, selectedTags.end());
    }

    const auto& query = tagPicker->searchQuery();
    const auto selectedTagNames = extractTagNamesThatStartWithQuery(selectedTags, query);
    tagPicker->displaySelectedTagNames(selectedTagNames);
}

namespace {

std::vector<std::string> extractTagNames(const std::vector<model::TagPtr>& tags)
{
    std::vector<std::string> tagNames;
    std::transform(tags.begin(), tags.end(), std::back_inserter(tagNames),
                   [](const model::TagPtr& tag) { return tag ? tag->name() : ""; });
    return tagNames;
}

std::vector<std::string> extractTagNamesThatStartWithQuery(const std::vector<model::TagPtr>& tags,
                                                           const std::string& query)
{
    const auto querySize = query.size();
    auto startsWithQuery = [&query, querySize](const model::TagPtr& tag) {
        return (tag && query == tag->name().substr(0, querySize));
    };

    std::vector<model::TagPtr> filteredTags;
    std::copy_if(tags.begin(), tags.end(), std::back_inserter(filteredTags), startsWithQuery);

    return extractTagNames(filteredTags);
}
} // namespace

} // namespace memo::ctrl

