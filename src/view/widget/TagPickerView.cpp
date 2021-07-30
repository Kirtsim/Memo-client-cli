#include "view/widget/TagPickerView.hpp"
#include "view/widget/TextEditView.hpp"
#include "view/widget/TextView.hpp"
#include "view/widget/ListView.hpp"
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
    TagPickerView& tagPicker_; // TODO: this smells
};

class SearchBarKeyFilter : public KeyFilter
{
public:
    explicit SearchBarKeyFilter(TagPickerView& tagPicker);
    ~SearchBarKeyFilter() override = default;
    bool filterKey(int key) override;
private:
    TagPickerView& tagPicker_; // TODO: this smells
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

class ViewFocusOperator
{
public:
    struct Focusable
    {
        TagPickerView::FocusedView view;
        std::function<bool()> isFocusable;
    };

    void stopFocus();

    TagPickerView::FocusedView viewInFocus() const;

    TagPickerView::FocusedView selectNext();

    TagPickerView::FocusedView selectPrev();

    void setFocusables(const std::vector<Focusable>& focusables);

private:
    size_t currentIdx_ = 0;
    bool focusing_ = false;
    std::vector<Focusable> focusables_;
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
    , confirmButton_(std::make_shared<TextView>(this))
    , cancelButton_(std::make_shared<TextView>(this))
    , createButton_(std::make_shared<TextView>(this))
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
    searchBar_->setKeyFilter(std::make_shared<SearchBarKeyFilter>(*this));
    searchBar_->setOnTextChangedListener(std::make_shared<QueryChangedListener>(*this));
    searchBar_->setBorder(curses::DefaultBorder());

    selectedTagsList_->setSelectionMark("  ");
    auto buttonBorder = curses::DefaultBorder();
    confirmButton_->setText("  Confirm  ");
    confirmButton_->setBorder(buttonBorder);
    confirmButton_->resizeToText();

    cancelButton_->setText("   Cancel  ");
    cancelButton_->setBorder(buttonBorder);
    cancelButton_->resizeToText();

    createButton_->setText("   Create  ");
    createButton_->setBorder(buttonBorder);
    createButton_->resizeToText();

    std::vector<ViewFocusOperator::Focusable> focusables {
            { kSearchBar,        [ ]() { return true; } },
            { kTagsList,         [&]() { return !tagsList_->empty(); } },
            { kSelectedTagsList, [&]() { return !selectedTagsList_->empty(); } },
            { kCreateButton,     [ ]() { return true; } },
            { kConfirmButton,    [ ]() { return true; } },
            { kCancelButton,     [ ]() { return true; } },
    };
    focusOperator_->setFocusables(focusables);
    initializeKeyMap();
}

void TagPickerView::initializeKeyMap()
{
    keyMap_.insert(std::make_pair(curses::Key::kEsc, [&]()
    {
        if (confirmCancelWithUser())
            focusOperator_->stopFocus();
        return (viewInFocus() == kNone);
    }));

    keyMap_.insert(std::make_pair(curses::Key::kEnter, [&]()
    {
        focusOperator_->stopFocus();
        return false;
    }));

    keyMap_.insert(std::make_pair(curses::Key::kTab, [&]()
    {
        auto oldView = focusOperator_->viewInFocus();
        focusOperator_->selectNext();

        if (oldView == kSearchBar && viewInFocus() != kSearchBar)
            searchBar_->looseFocus();
        return true;
    }));

    keyMap_.insert(std::make_pair(curses::Key::kShiftTab, [&]()
    {
        auto oldView = focusOperator_->viewInFocus();
        focusOperator_->selectPrev();

        if (oldView == kSearchBar && viewInFocus() != kSearchBar)
            searchBar_->looseFocus();
        return true;
    }));

    keyMap_.insert(std::make_pair(curses::Key::kSpace, [&]()
    {
        if (viewInFocus() == kTagsList)
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
        if (viewInFocus() == kTagsList)
            tagsList_->selectPrev();
        else if (viewInFocus() == kSelectedTagsList)
            selectedTagsList_->selectPrev();
        return true;
    }));

    keyMap_.insert(std::make_pair(curses::Key::kDown, [&]()
    {
        if (viewInFocus() == kTagsList)
            tagsList_->selectNext();
        else if (viewInFocus() == kSelectedTagsList)
            selectedTagsList_->selectNext();
        return true;
    }));
}

TagPickerView::~TagPickerView() = default;

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

bool TagPickerView::display()
{
    refresh();
    while (viewInFocus() != kNone)
    {
        switch(viewInFocus())
        {
            case kSearchBar: focusSearchBar(); break;
            case kTagsList: focusTagsList(); break;
            case kSelectedTagsList: focusSelectedTagsList(); break;
            case kCreateButton: readCreateButtonInput(); break;
            case kConfirmButton: readConfirmButtonInput(); break;
            case kCancelButton: readCancelButtonInput(); break;
            default: { focusOperator_->stopFocus(); }
        }
        refresh();
    }
    parentRequestOnRefresh();
    return false;
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

bool TagPickerView::processSearchBarKey(const int key)
{
    auto keyIter = keyMap_.find(key);
    if (keyIter != std::end(keyMap_))
    {
        auto keyFunc = keyIter->second;
        keyFunc();
        if (viewInFocus() != TagPickerView::kCancelButton)
            searchBar_->looseFocus();
        return true;
    }
    return false;
}

void TagPickerView::readTagsListInput()
{
    curses::KeyPad(tagsList_->getWindow(), ENABLE);
    const bool wasCursorVisible = curses::CursorVisible(false);

    while (viewInFocus() == kTagsList)
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
    while (viewInFocus() == kSelectedTagsList)
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
    // TODO: think about reducing this "readXXX" code.
    curses::KeyPad(createButton_->getWindow(), ENABLE);
    const bool wasCursorVisible = curses::CursorVisible(false);
    createButton_->setBorder(selectedViewBorder());
    ForceRefresh(createButton_);

    while (viewInFocus() == kCreateButton)
    {
        auto key = curses::ReadChar(createButton_->getWindow());
        auto keyIter = keyMap_.find(key);
        if (keyIter != std::end(keyMap_))
        {
            auto keyFunc = keyIter->second;
            keyFunc();
        }
    }

    createButton_->setBorder(curses::DefaultBorder());
    ForceRefresh(createButton_);
    curses::CursorVisible(wasCursorVisible);
    curses::KeyPad(createButton_->getWindow(), DISABLE);
}

void TagPickerView::readConfirmButtonInput()
{
    curses::KeyPad(confirmButton_->getWindow(), ENABLE);
    const bool wasCursorVisible = curses::CursorVisible(false);
    confirmButton_->setBorder(selectedViewBorder());
    ForceRefresh(confirmButton_);

    while (viewInFocus() == kConfirmButton)
    {
        auto key = curses::ReadChar(confirmButton_->getWindow());
        auto keyIter = keyMap_.find(key);
        if (keyIter != std::end(keyMap_))
        {
            auto keyFunc = keyIter->second;
            keyFunc();
        }
    }

    confirmButton_->setBorder(curses::DefaultBorder());
    ForceRefresh(confirmButton_);
    curses::CursorVisible(wasCursorVisible);
    curses::KeyPad(confirmButton_->getWindow(), DISABLE);
}

void TagPickerView::readCancelButtonInput()
{
    curses::KeyPad(cancelButton_->getWindow(), ENABLE);
    const bool wasCursorVisible = curses::CursorVisible(false);
    cancelButton_->setBorder(selectedViewBorder());
    ForceRefresh(cancelButton_);

    while (viewInFocus() == kCancelButton)
    {
        auto key = curses::ReadChar(cancelButton_->getWindow());
        auto keyIter = keyMap_.find(key);
        if (keyIter != std::end(keyMap_))
        {
            auto keyFunc = keyIter->second;
            keyFunc();
        }
    }

    cancelButton_->setBorder(curses::DefaultBorder());
    ForceRefresh(cancelButton_);
    curses::CursorVisible(wasCursorVisible);
    curses::KeyPad(cancelButton_->getWindow(), DISABLE);
}

TagPickerView::FocusedView TagPickerView::viewInFocus()
{
    return focusOperator_->viewInFocus();
}

////////////////////////////////////////////////
///         ViewFocusOperator
////////////////////////////////////////////////

void ViewFocusOperator::stopFocus()
{
    focusing_ = false;
}

TagPickerView::FocusedView ViewFocusOperator::viewInFocus() const
{
    if (focusing_ && currentIdx_ < focusables_.size())
        return focusables_[currentIdx_].view;
    return TagPickerView::kNone;
}

TagPickerView::FocusedView ViewFocusOperator::selectNext()
{
    if (!focusables_.empty())
    {
        const auto startIdx = currentIdx_;
        do
        {
            currentIdx_ = (currentIdx_ + 1) % focusables_.size();
        }
        while (currentIdx_ != startIdx && !focusables_[currentIdx_].isFocusable());

        return focusables_[currentIdx_].view;
    }
    return TagPickerView::kNone;
}

TagPickerView::FocusedView ViewFocusOperator::selectPrev()
{
    if (!focusables_.empty())
    {
        const auto startIdx = currentIdx_;
        do
        {
            currentIdx_ = std::min(currentIdx_ - 1, focusables_.size() - 1);
        }
        while (currentIdx_ != startIdx && !focusables_[currentIdx_].isFocusable());

        return focusables_[currentIdx_].view;
    }
    return TagPickerView::kNone;
}

void ViewFocusOperator::setFocusables(const std::vector<Focusable>& focusables)
{
    focusables_ = focusables;
    currentIdx_ = 0;
    focusing_ = (!focusables_.empty());
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
///         SearchBarKeyFilter
////////////////////////////////////////////////
SearchBarKeyFilter::SearchBarKeyFilter(TagPickerView& tagPicker)
        : tagPicker_(tagPicker)
{
}

bool SearchBarKeyFilter::filterKey(const int key)
{
    return tagPicker_.processSearchBarKey(key);
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
