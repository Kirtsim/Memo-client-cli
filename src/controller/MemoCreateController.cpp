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
#include <map>

namespace memo::ctrl {

namespace {
    std::vector<std::string> extractTagNames(const std::vector<model::TagPtr>& tags);
    std::vector<std::string> extractTagNamesWithPrefix(const std::vector<model::TagPtr>& tags,
                                                       const std::string& query);
} // namespace

class SortedTagCollection
{
public:
    void add(const model::TagPtr& tag);

    void add(const std::vector<model::TagPtr>& tags);

    model::TagPtr remove(const std::string& name);

    void clear();

    model::TagPtr find(const std::string& name) const;

    bool contains(const std::string& name) const;

    std::vector<model::TagPtr> listAll() const;

private:
    std::string toLowercase(const std::string& str) const;

private:
    std::map<std::string, model::TagPtr> tags_;
};


MemoCreateController::MemoCreateController(const ResourcesPtr_t& resources)
    : BaseController(resources)
    , tagsCollection_(std::make_unique<SortedTagCollection>())
    , selectedTagsCollection_(std::make_unique<SortedTagCollection>())
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

    fetchTags();
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
    memo->setTags(selectedTagsCollection_->listAll());
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

void MemoCreateController::fetchTags()
{
    auto service = getResources()->tagService();
    if (!service)
    {
        // TODO: Log a message.
        return;
    }
    remote::ListTagsRequestBuilder requestBuilder;
    requestBuilder.setUuid("memo-create-controller-fetch-tags") // TODO: generate a real uuid.
                  .setView(remote::ModelView::kMinimalView);
    auto response = service->listTags(requestBuilder.build());
    if (!response || response->status() == remote::ResponseStatus::kError)
    {
        // TODO: Log a message.
        return;
    }

    const auto& responseData = response->data();
    tagsCollection_->add(responseData.tags());
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

    auto tagSelection = *selectedTagsCollection_;
    const auto allTagNames = extractTagNames(tagsCollection_->listAll());
    const auto allSelectedTagNames = extractTagNames((tagSelection.listAll()));
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
        *selectedTagsCollection_ = tagSelection;
        view()->displayTagNames(extractTagNames(tagSelection.listAll()));
    }
    ui::tools::ForceRefresh(view());
}

void MemoCreateController::onTagSearchQueryChanged(const std::string& query,
                                                   const SortedTagCollection& selectedTags)
{
    const auto queriedTagNames = extractTagNamesWithPrefix(tagsCollection_->listAll(), query);
    const auto selectedTagNames = extractTagNamesWithPrefix(selectedTags.listAll(), query);
    tagPickerView_->displayTags(queriedTagNames);
    tagPickerView_->displaySelectedTagNames(selectedTagNames);
    tagPickerView_->refresh();
}

void MemoCreateController::onTagSelectionChanged(const std::string& tagName, bool selected,
                                                 SortedTagCollection& selectedTags)
{
    if (selected)
    {
        if (!selectedTags.contains(tagName))
        {
            const auto tag = tagsCollection_->find(tagName);
            if (tag)
                selectedTags.add(tag);
        }
    }
    else
    {
        selectedTags.remove(tagName);
    }

    const auto& query = tagPickerView_->searchQuery();
    const auto selectedTagNames = extractTagNamesWithPrefix(selectedTags.listAll(), query);
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
    createTagView_->setInfoTextVisible(tagsCollection_->contains(suggestedTagName));
    ui::tools::ForceRefresh(createTagView_);

    createTagView_->readInput();

    ui::tools::ForceRefresh(view());
    ui::tools::ForceRefresh(tagPickerView_);
}

void MemoCreateController::onConfirmNewTagButtonClicked()
{
    const auto& tagName = createTagView_->tagName();
    if (tagName.empty() || tagsCollection_->contains(tagName))
        return;
    auto tag = std::make_shared<model::Tag>();
    tag->setName(tagName);
    tag->setTimestamp(static_cast<unsigned long>(std::time(nullptr)));
    if (auto newTag = createTag(tag))
    {
        tagsCollection_->add(newTag);
        // TODO: update tagPickerView
        ui::MessageDialog::Display("Tag \"" + tagName + "\" created.", view().get());
    }
    else
    {
        ui::MessageDialog::Display("Failed to create tag \"" + tagName + "\".", view().get());
    }
    ui::tools::ForceRefresh(createTagView_);
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
    createTagView_->setInfoTextVisible(tagsCollection_->contains(tagName));
    createTagView_->refresh();
}

void SortedTagCollection::add(const model::TagPtr& tag)
{
    if (tag)
    {
        const auto key = toLowercase(tag->name());
        tags_.insert_or_assign(key, tag);
    }
}

void SortedTagCollection::add(const std::vector<model::TagPtr>& tags)
{
    for (const auto& tag : tags)
        add(tag);
}

model::TagPtr SortedTagCollection::remove(const std::string& name)
{
    model::TagPtr removedTag = nullptr;
    const auto key = toLowercase(name);
    auto it = tags_.find(key);
    if (it != std::end(tags_))
    {
        removedTag = it->second;
        tags_.erase(it);
    }
    return removedTag;
}

void SortedTagCollection::clear()
{
    tags_.clear();
}

model::TagPtr SortedTagCollection::find(const std::string& name) const
{
    const auto it = tags_.find(toLowercase(name));
    return (it != std::end(tags_)) ? it->second : nullptr;
}

bool SortedTagCollection::contains(const std::string& name) const
{
    return find(name) != nullptr;
}

std::vector<model::TagPtr> SortedTagCollection::listAll() const
{
    std::vector<model::TagPtr> sortedTags(tags_.size(), nullptr);
    size_t index = 0;
    for (auto iter = tags_.begin(); iter != std::end(tags_); ++iter)
        sortedTags[index++] = iter->second;

    return sortedTags;
}

std::string SortedTagCollection::toLowercase(const std::string& str) const
{
    std::string result = str;
    std::transform(str.begin(), str.end(), result.begin(), [](const char c) { return std::tolower(c); });
    return result;
}

namespace {

std::vector<std::string> extractTagNames(const std::vector<model::TagPtr>& tags)
{
    std::vector<std::string> tagNames;
    std::transform(tags.begin(), tags.end(), std::back_inserter(tagNames),
                   [](const model::TagPtr& tag) { return tag ? tag->name() : ""; });
    return tagNames;
}

std::vector<std::string> extractTagNamesWithPrefix(const std::vector<model::TagPtr>& tags, const std::string& query)
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

