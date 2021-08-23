#include "controller/MemoCreateController.hpp"
#include "view/TagCreateView.hpp"
#include "view/widget/TextEditView.hpp"
#include "view/widget/TagPickerView.hpp"
#include "view/dialog/MessageDialog.hpp"
#include "view/tools/Tools.hpp"
#include "manager/ControllerManager.hpp"
#include "remote/MemoService.hpp"
#include "remote/TagService.hpp"
#include "remote/AddMemoRequest.hpp"
#include "remote/AddTagRequest.hpp"
#include "remote/ServiceResponse.hpp"
#include "model/Memo.hpp"
#include "model/ModelDefs.hpp"
#include "remote/ListTagsRequest.hpp"
#include "remote/ListTagsResponseData.hpp"
#include "remote/AddTagResponseData.hpp"

#include "ncurses/keys.hpp"

#include <ctime>

namespace memo::ctrl {

namespace {
    std::vector<std::string> extractTagNames(const std::vector<model::TagPtr>& tags);
    std::vector<std::string> extractTagNamesThatStartWithQuery(const std::vector<model::TagPtr>& tags,
                                                               const std::string& query);
    bool containsTagWithName(const std::vector<model::TagPtr>& tags, const std::string& name);
} // namespace

MemoCreateController::MemoCreateController(const ResourcesPtr_t& resources)
    : BaseController(resources)
    , tagPickerView_(std::make_shared<ui::TagPickerView>())
    , createTagView_(std::make_shared<ui::TagCreateView>())
{
    auto view = std::make_shared<ui::MemoCreateView>();
    setView(view);
    tagPickerView_->setParent(view.get());
    createTagView_->setParent(view.get());

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
    view()->focusSubView(ui::MemoCreateView::kNone);
    view()->looseFocus();
    getResources()->controllerManager()->pop();
}

void MemoCreateController::pickTags()
{
    tagPickerView_->setWidth(static_cast<int>(view()->getWidth() * 0.3));
    tagPickerView_->setHeight(static_cast<int>(view()->getHeight() * 0.7));
    ui::tools::CenterComponent(*tagPickerView_, Center::CENTER, *view());

    std::vector<model::TagPtr> tagSelection = selectedTags_;
    const auto allTagNames = extractTagNames(tags_);
    const auto allSelectedTagNames = extractTagNames((tagSelection));
    tagPickerView_->displayTags(allTagNames);
    tagPickerView_->displaySelectedTagNames(allSelectedTagNames);
    tagPickerView_->setSearchBarChangedCallback([&](const std::string& query) {
        onTagSearchQueryChanged(query, tagSelection);
    });

    tagPickerView_->setTagSelectionChangedCallback([&](const std::string& tagName, bool selected) {
        onTagSelectionChanged(tagName, selected, tagSelection);
    });

    tagPickerView_->setCreateButtonClickedCallback([&](const std::string& suggestedTagName) {
        onCreateTagButtonClicked(suggestedTagName);
    });

    const auto tagsSelected = tagPickerView_->display();
    if (tagsSelected)
    {
        selectedTags_ = tagSelection;
        view()->displayTagNames(extractTagNames(selectedTags_));
    }
    view()->refreshOnRequest();
    view()->refresh();
}

void MemoCreateController::onTagSearchQueryChanged(const std::string& query,
                                                   const std::vector<model::TagPtr>& selectedTags)
{
    const auto queriedTagNames = extractTagNamesThatStartWithQuery(tags_, query);
    const auto selectedTagNames = extractTagNamesThatStartWithQuery(selectedTags, query);
    tagPickerView_->displayTags(queriedTagNames);
    tagPickerView_->displaySelectedTagNames(selectedTagNames);
    tagPickerView_->refresh();
}

void MemoCreateController::onTagSelectionChanged(const std::string& tagName, bool selected,
                                                 std::vector<model::TagPtr>& selectedTags)
{
    if (selected)
    {
        if (!containsTagWithName(selectedTags, tagName))
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

    const auto& query = tagPickerView_->searchQuery();
    const auto selectedTagNames = extractTagNamesThatStartWithQuery(selectedTags, query);
    tagPickerView_->displaySelectedTagNames(selectedTagNames);
}

void MemoCreateController::onCreateTagButtonClicked(const std::string& suggestedTagName)
{
    createTagView_->setHeight(30);
    createTagView_->setWidth(static_cast<int>(view()->getWidth() * 0.25));
    ui::tools::CenterComponent(*createTagView_, CENTER, *view());
    createTagView_->setOnConfirmButtonClicked([&](int) { onConfirmNewTagButtonClicked(); });
    createTagView_->setOnCancelButtonClicked([&](int) { createTagView_->looseFocus(); });
    createTagView_->setOnTagNameChanged([&](const std::string& tagName) { onCreateNewTagNameChanged(tagName); });
    createTagView_->setTagName(suggestedTagName);
    createTagView_->setInfoTextVisible(containsTagWithName(tags_, suggestedTagName));
    createTagView_->refreshOnRequest();
    createTagView_->refresh();

    createTagView_->readInput();

    view()->refreshOnRequest();
    view()->refresh();
    tagPickerView_->refreshOnRequest();
    tagPickerView_->refresh();
}

void MemoCreateController::onConfirmNewTagButtonClicked()
{
    const auto& tagName = createTagView_->tagName();
    if (tagName.empty() || containsTagWithName(tags_, tagName))
        return;
    auto tag = std::make_shared<model::Tag>();
    tag->setName(tagName);
    tag->setTimestamp(static_cast<unsigned long>(std::time(nullptr)));
    if (auto newTag = createTag(tag))
    {
        tags_.emplace_back(newTag);
        // TODO: update tagPickerView
        ui::MessageDialog::Display("Tag \"" + tagName + "\" created.", view().get());
    }
    else
    {
        ui::MessageDialog::Display("Failed to create tag \"" + tagName + "\".", view().get());
    }
    createTagView_->refreshOnRequest();
    createTagView_->refresh();
}

model::TagPtr MemoCreateController::createTag(const model::TagPtr& tag)
{
    if (!tag)
        return nullptr;
    auto service = getResources()->tagService();
    if (!service)
    {
        //TODO: log
        return nullptr;
    }
    remote::AddTagRequestBuilder requestBuilder;
    requestBuilder.setUuid("memo-create-controller-add-tag")
                  .setTag(tag);
    auto response = service->addTag(requestBuilder.build());
    if (!response || response->status() == remote::ResponseStatus::kError)
    {
        // TODO: log
        return nullptr;
    }

    return response->data().tag();
}

void MemoCreateController::onCreateNewTagNameChanged(const std::string& tagName)
{
    createTagView_->setInfoTextVisible(containsTagWithName(tags_, tagName));
    createTagView_->refresh();
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

bool containsTagWithName(const std::vector<model::TagPtr>& tags, const std::string& name)
{
    auto namesMatch = [&name](const model::TagPtr& tag) {
        return tag && tag->name() == name;
    };
    return std::find_if(tags.begin(), tags.end(), namesMatch) != std::end(tags);
}
} // namespace

} // namespace memo::ctrl

