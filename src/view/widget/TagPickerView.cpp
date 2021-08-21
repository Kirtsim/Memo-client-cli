#include "view/widget/TagPickerView.hpp"
#include "view/widget/TextEditView.hpp"
#include "view/widget/ButtonView.hpp"
#include "view/widget/ListView.hpp"
#include "view/tools/ViewFocusOperator.hpp"
#include "view/dialog/ConfirmDialog.hpp"
#include "ncurses/functions.hpp"
#include "ncurses/keys.hpp"

#include <set>

namespace memo::ui {

namespace {
    const int kSearchBarHeight = 3;

    Border selectedViewBorder();
    void ForceRefresh(const std::shared_ptr<View>& view);

    std::vector<std::string> extractTagNames(const std::vector<ListItemPtr>& items);
    std::vector<ListItemPtr> toListItems(const std::vector<std::string>& tagNames);
    void markAsSelectedIfPresent(const std::vector<ListItemPtr>& presentIn, std::vector<ListItemPtr> forMarking);
} // namespace

class QueryChangedListener : public OnTextChangedListener
{
public:
    explicit QueryChangedListener(TagPickerView& tagPicker);
    ~QueryChangedListener() override;
    void onTextChanged(const std::string& text) override;

private:
    TagPickerView& tagPicker_;
};

class TextItem : public ListItem
{
public:
    explicit TextItem(const std::string& text);
    ~TextItem() override = default;
    std::string text() const override;
    const std::string& tagName() const;
    bool isSelected() const;
    void setSelected(bool select);
private:
    std::string text_;
    bool isSelected_ = false;
};

TagPickerView::TagPickerView(IComponent* parent)
    : TagPickerView(Size(), Position(), parent)
{
}

TagPickerView::TagPickerView(const Size& size, IComponent* parent)
    : TagPickerView(size, Position(), parent)
{
}

TagPickerView::TagPickerView(const Size& size, const Position& position, IComponent* parent)
    : BaseView(size, position, parent)
    , searchBar_(std::make_shared<TextEditView>(this))
    , tagsList_(std::make_shared<ListView>(this))
    , selectedTagsList_(std::make_shared<ListView>(this))
    , confirmButton_(std::make_shared<ButtonView>(this))
    , cancelButton_(std::make_shared<ButtonView>(this))
    , createButton_(std::make_shared<ButtonView>(this))
    , focusOperator_(std::make_unique<ViewFocusOperator>())
{
    this->setBorder(Border::NoBorder());
    registerSubView(searchBar_);
    registerSubView(tagsList_);
    registerSubView(selectedTagsList_);
    registerSubView(confirmButton_);
    registerSubView(cancelButton_);
    registerSubView(createButton_);
    searchBar_->setSize({ Height(kSearchBarHeight), Width(size.width) });
    searchBar_->setKeyFilter([&](int key) { return processSearchBarKey(key); });
    searchBar_->setOnTextChangedListener(std::make_shared<QueryChangedListener>(*this));
    searchBar_->setBorder(curses::DefaultBorder());

    selectedTagsList_->setSelectionMark("  ");

    auto keyFilter = [&](int key) { return processKey(key); };
    confirmButton_->setText("  Confirm  ");
    confirmButton_->setBorder(curses::DefaultBorder());
    confirmButton_->setInFocusBorder(selectedViewBorder());
    confirmButton_->resizeToText();
    confirmButton_->setOnButtonClicked([&](int /*key*/) { onConfirmButtonClicked(); });
    confirmButton_->setKeyFilter(keyFilter);

    cancelButton_->setText("   Cancel  ");
    cancelButton_->setBorder(curses::DefaultBorder());
    cancelButton_->setInFocusBorder(selectedViewBorder());
    cancelButton_->resizeToText();
    cancelButton_->setOnButtonClicked([&](int /*key*/) { onCancelButtonClicked(); });
    cancelButton_->setKeyFilter(keyFilter);

    createButton_->setText("   Create  ");
    createButton_->setBorder(curses::DefaultBorder());
    createButton_->setInFocusBorder(selectedViewBorder());
    createButton_->resizeToText();
    createButton_->setOnButtonClicked([&](int /*key*/) { onCreateButtonClicked(); });
    createButton_->setKeyFilter(keyFilter);

    std::vector<ViewFocusOperator::Focusable> focusables {
            { searchBar_,        [ ]() { return true; } },
            { tagsList_,         [&]() { return !tagsList_->empty(); } },
            { selectedTagsList_, [&]() { return !selectedTagsList_->empty(); } },
            { createButton_,     [ ]() { return true; } },
            { confirmButton_,    [ ]() { return true; } },
            { cancelButton_,     [ ]() { return true; } },
    };
    focusOperator_->setFocusables(focusables);
    initializeKeyMap();
}

TagPickerView::~TagPickerView() = default;

void TagPickerView::initializeKeyMap()
{
    keyMap_.insert(std::make_pair(curses::Key::kEsc, [&]()
    {
        if (confirmCancelWithUser())
        {
            focusOperator_->stopFocus();
            return true;
        }
        refreshOnRequest();
        refresh();
        return false;
    }));

    keyMap_.insert(std::make_pair(curses::Key::kTab, [&]()
    {
        auto oldViewInFocus = focusOperator_->viewInFocus();
        focusOperator_->selectNext();
        return true;
    }));

    keyMap_.insert(std::make_pair(curses::Key::kShiftTab, [&]()
    {
        auto oldView = focusOperator_->viewInFocus();
        focusOperator_->selectPrev();
        return true;
    }));

    keyMap_.insert(std::make_pair(curses::Key::kSpace, [&]()
    {
        if (viewInFocus() == tagsList_)
        {
            if (auto currentItem = std::dynamic_pointer_cast<TextItem>(tagsList_->selected()))
            {
                const bool newSelectionState = !currentItem->isSelected();
                if (tagSelectionChangedCallback_)
                {
                    tagSelectionChangedCallback_(currentItem->tagName(), newSelectionState);
                    ForceRefresh(selectedTagsList_);
                }
                currentItem->setSelected(newSelectionState);
                ForceRefresh(tagsList_);
            }
        }
        return true;
    }));

    keyMap_.insert(std::make_pair(curses::Key::kUp, [&]()
    {
        if (viewInFocus() == tagsList_)
            tagsList_->selectPrev();
        else if (viewInFocus() == selectedTagsList_)
            selectedTagsList_->selectPrev();
        return true;
    }));

    keyMap_.insert(std::make_pair(curses::Key::kDown, [&]()
    {
        if (viewInFocus() == tagsList_)
            tagsList_->selectNext();
        else if (viewInFocus() == selectedTagsList_)
            selectedTagsList_->selectNext();
        return true;
    }));
}

std::string TagPickerView::searchQuery() const
{
    return searchBar_->text();
}

void TagPickerView::displayTags(const std::vector<std::string>& tagNames)
{
    const auto items = toListItems(tagNames);
    tagsList_->setItems(items);
    markAsSelectedIfPresent(selectedTagsList_->items(), items);
}

std::vector<std::string> TagPickerView::displayedTags() const
{
    return extractTagNames(tagsList_->items());
}

void TagPickerView::displaySelectedTagNames(const std::vector<std::string>& tagNames)
{
    const auto items = toListItems(tagNames);
    selectedTagsList_->setItems(items);
    markAsSelectedIfPresent(items, tagsList_->items());
}

std::vector<std::string> TagPickerView::displayedSelectedTags() const
{
    return extractTagNames(selectedTagsList_->items());
}

void TagPickerView::setSearchBarChangedCallback(const SearchBarChangedCallback& callback)
{
    searchQueryChangedCallback_ = callback;
}

void TagPickerView::setTagSelectionChangedCallback(const TagSelectionChangedCallback& callback)
{
    tagSelectionChangedCallback_ = callback;
}

void TagPickerView::setCreateButtonClickedCallback(const CreateButtonClickedCallback& callback)
{
    createButtonClickedCallback_ = callback;
}

bool TagPickerView::display()
{
    focusOperator_->resetFocus();
    refreshOnRequest();
    refresh();
    while (auto view = viewInFocus())
    {
        if (view == searchBar_)
            focusSearchBar();
        else if (view == tagsList_)
            focusTagsList();
        else if (view == selectedTagsList_)
            focusSelectedTagsList();
        else if (view == createButton_)
            readCreateButtonInput();
        else if (view == confirmButton_)
            readConfirmButtonInput();
        else if (view == cancelButton_)
            readCancelButtonInput();
        else
            focusOperator_->stopFocus();
        refresh();
    }
    parentRequestOnRefresh();
    return selectionConfirmed_;
}

void TagPickerView::focusTagsList()
{
    tagsList_->setBorder(selectedViewBorder());
    ForceRefresh(tagsList_);
    readTagsListInput();
    tagsList_->setBorder(curses::DefaultBorder());
    ForceRefresh(tagsList_);
}

void TagPickerView::focusSelectedTagsList()
{
    selectedTagsList_->setBorder(selectedViewBorder());
    ForceRefresh(selectedTagsList_);
    readSelectedTagsListInput();
    selectedTagsList_->setBorder(curses::DefaultBorder());
    ForceRefresh(selectedTagsList_);
}

void TagPickerView::focusSearchBar()
{
    searchBar_->setBorder(selectedViewBorder());
    ForceRefresh(searchBar_);
    searchBar_->readInput();
    searchBar_->setBorder(curses::DefaultBorder());
    ForceRefresh(searchBar_);
}

void TagPickerView::displayContent()
{
    const auto baseSize = getSize();
    searchBar_->setSize({ Height(kSearchBarHeight), Width(baseSize.width) });

    const auto buttonHeight = confirmButton_->getHeight();

    const auto listHeight = baseSize.height - kSearchBarHeight - buttonHeight - 1;
    const auto listWidth = baseSize.width / 2;
    tagsList_->setSize({ Height(listHeight), Width(std::max(listWidth, baseSize.width - listWidth)) });
    selectedTagsList_->setSize({ Height(listHeight), Width(baseSize.width - tagsList_->getWidth()) });

    searchBar_->setPosition({ PosX(0), PosY(0) });
    tagsList_->setPosition({ PosX(0), PosY(kSearchBarHeight)});
    selectedTagsList_->setPosition({ PosX(baseSize.width - selectedTagsList_->getWidth()), PosY(kSearchBarHeight) });

    const auto buttonPosY = baseSize.height - confirmButton_->getHeight() - 1;
    cancelButton_->setX(baseSize.width - cancelButton_->getWidth());
    cancelButton_->setY(buttonPosY);
    confirmButton_->setX(cancelButton_->getX() - 1 - confirmButton_->getWidth());
    confirmButton_->setY(buttonPosY);
    createButton_->setPosition({ PosX(0), PosY(buttonPosY) });

    markAsSelectedIfPresent(selectedTagsList_->items(), tagsList_->items());
}

bool TagPickerView::confirmCancelWithUser()
{
    return ConfirmDialog::Display("Selected tags will be discarded. Continue?", this);
}

std::shared_ptr<View> TagPickerView::viewInFocus() const
{
    return focusOperator_->viewInFocus();
}

bool TagPickerView::processSearchBarKey(const int key)
{
    auto keyIter = keyMap_.find(key);
    if (keyIter != std::end(keyMap_))
    {
        auto keyFunc = keyIter->second;
        keyFunc();
        if (viewInFocus() != cancelButton_)
            searchBar_->looseFocus();
        return true;
    }
    return false;
}

void TagPickerView::readTagsListInput()
{
    curses::KeyPad(tagsList_->getWindow(), ENABLE);
    const bool wasCursorVisible = curses::CursorVisible(false);

    while (viewInFocus() == tagsList_)
    {
        auto key = curses::ReadChar(tagsList_->getWindow());
        auto keyIter = keyMap_.find(key);
        if (keyIter != std::end(keyMap_))
        {
            auto keyFunc = keyIter->second;
            keyFunc();
            tagsList_->refresh();
        }
    }

    curses::CursorVisible(wasCursorVisible);
    curses::KeyPad(tagsList_->getWindow(), DISABLE);
}

void TagPickerView::readSelectedTagsListInput()
{
    curses::KeyPad(selectedTagsList_->getWindow(), ENABLE);
    const bool wasCursorVisible = curses::CursorVisible(false);
    selectedTagsList_->setSelectionMark("* ");
    ForceRefresh(selectedTagsList_);
    while (viewInFocus() == selectedTagsList_)
    {
        auto key = curses::ReadChar(selectedTagsList_->getWindow());
        auto keyIter = keyMap_.find(key);
        if (keyIter != std::end(keyMap_))
        {
            auto keyFunc = keyIter->second;
            keyFunc();
            selectedTagsList_->refresh();
        }
    }

    selectedTagsList_->setSelectionMark("  ");
    ForceRefresh(selectedTagsList_);
    curses::CursorVisible(wasCursorVisible);
    curses::KeyPad(selectedTagsList_->getWindow(), DISABLE);
}

void TagPickerView::readCreateButtonInput()
{
    createButton_->readInput();
}

void TagPickerView::readConfirmButtonInput()
{
    confirmButton_->readInput();
}

void TagPickerView::readCancelButtonInput()
{
    cancelButton_->readInput();
}

void TagPickerView::onCreateButtonClicked()
{
    if (createButtonClickedCallback_)
        createButtonClickedCallback_(searchBar_->text());
}

void TagPickerView::onConfirmButtonClicked()
{
    selectionConfirmed_ = true;
    confirmButton_->looseFocus();
    focusOperator_->stopFocus();
}

void TagPickerView::onCancelButtonClicked()
{
    if (confirmCancelWithUser())
    {
        confirmButton_->looseFocus();
        focusOperator_->stopFocus();
    }
}

bool TagPickerView::processKey(int key)
{
    auto keyIter = keyMap_.find(key);
    const bool keyRecognized = (keyIter != std::end(keyMap_));
    if (keyRecognized)
    {
        auto& keyFunc = keyIter->second;
        keyFunc();
        return true;
    }
    return false;
}

////////////////////////////////////////////////
///         QueryChangedListener
////////////////////////////////////////////////
QueryChangedListener::QueryChangedListener(TagPickerView& tagPicker)
    : tagPicker_(tagPicker)
{
}

QueryChangedListener::~QueryChangedListener() = default;

void QueryChangedListener::onTextChanged(const std::string& text)
{
    if (auto callback = tagPicker_.searchQueryChangedCallback_)
        callback(text);
}

////////////////////////////////////////////////
///         TextItem
/// ////////////////////////////////////////////
TextItem::TextItem(const std::string& text)
        : text_(text)
{
}

std::string TextItem::text() const
{
    return isSelected_ ? (">> " + text_) : text_;
}

const std::string& TextItem::tagName() const
{
    return text_;
}

bool TextItem::isSelected() const
{
    return isSelected_;
}

void TextItem::setSelected(bool select)
{
    isSelected_ = select;
}

namespace {

Border selectedViewBorder()
{
    auto border = curses::DefaultBorder();
    border.bottom = border.top = '-';
    border.left = border.right = '|';
    return border;
}

void ForceRefresh(const std::shared_ptr<View>& view)
{
    if (view)
    {
        view->refreshOnRequest();
        view->refresh();
    }
}

std::vector<std::string> extractTagNames(const std::vector<ListItemPtr>& items)
{
    const auto toString = [](const ListItemPtr& item) { return item ? item->text() : ""; };
    std::vector<std::string> tagNames;

    std::transform(items.begin(), items.end(), std::back_inserter(tagNames), toString);

    return tagNames;
}

std::vector<ListItemPtr> toListItems(const std::vector<std::string>& tagNames)
{
    const auto toTextItem = [](const std::string& name){ return std::make_shared<TextItem>(name); };

    std::vector<ListItemPtr> items;
    std::transform(tagNames.begin(), tagNames.end(), std::back_inserter(items), toTextItem);

    return items;
}

void markAsSelectedIfPresent(const std::vector<ListItemPtr>& presentIn, std::vector<ListItemPtr> forMarking)
{
    const auto selectedTagNames = extractTagNames(presentIn);
    std::unordered_set<std::string> selectedTagsSet(selectedTagNames.begin(), selectedTagNames.end());
    for (auto& listItem : forMarking)
    {
        if (auto tagItem = std::dynamic_pointer_cast<TextItem>(listItem))
        {
            const bool isInSelected = selectedTagsSet.find(tagItem->tagName()) != std::end(selectedTagsSet);
            tagItem->setSelected(isInSelected);
        }
    }
}

} // namespace
} // namespace memo::ui
