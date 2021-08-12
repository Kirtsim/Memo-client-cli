#pragma once
#include "view/BaseView.hpp"

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>

namespace memo::ui {

class TextEditView;
class ButtonView;
class ListView;
class ViewFocusOperator;

using SearchBarChangedCallback = std::function<void(const std::string&)>;
using TagSelectionChangedCallback = std::function<void(const std::string&, bool)>;
using CreateButtonClickedCallback = std::function<void(const std::string&)>;

/// @brief A widget with visual components for searching and selecting Tags.
///
/// UI components of this widget:
/// 1) "Search bar" - used by the user to write search queries.
/// 2) "Left-side Tag column" - lists tag names that resulted from the search.
/// 3) "Right-side Tag column" - lists tag names that have been selected by the user.
/// 4) "Create button" - the user can request to create a new tag.
/// 5) "Confirm button" - to confirm the tag selection.
/// 6) "Cancel button" - to cancel all (selection) changes.
///
/// Widget is displayed by calling TagPickerView::display() function that returns
/// true if the user clicked the "Confirm button" or false in case the user decides to
/// discard all changes by clicking the "Cancel button" or "Esc" key on keyboard.
class TagPickerView : public BaseView
{
public:
    explicit TagPickerView(IComponent* parent=nullptr);

    explicit TagPickerView(const Size& size, IComponent* parent=nullptr);

    TagPickerView(const Size& size, const Position& position, IComponent* parent=nullptr);

    ~TagPickerView() override;

    /// @brief A getter method for the search query that is being typed in the search bar.
    /// @return text (search query) currently displayed in the search bar.
    std::string searchQuery() const;

    /// @brief Populates the left-hand side column of Tags with the given tag names.
    ///
    /// The affected column should list tags that match the query in the search bar.
    /// @param tagNames list of tag names to be displayed in the left-hand side column.
    void displayTags(const std::vector<std::string>& tagNames);

    /// @brief Getter method for accessing tag names currently listed in the left-hand
    /// side column. This column should list tag names that match the search query.
    /// @return list of tag names currently being displayed in the left-hand side column.
    std::vector<std::string> displayedTags() const;

    /// @brief Populates the right-hand side column of Tags with the given tag names.
    ///
    /// The affected column should list tags that have been selected by the user. Similar
    /// to the left-hand side column, these tags can be filtered by the search query.
    /// @param tagNames list of tag names to be displayed in the right-hand side column.
    void displaySelectedTagNames(const std::vector<std::string>& tagNames);

    /// @brief Getter method for accessing tag names currently listed in the right-hand
    /// side column. This column should list tag names that that have been selected (and
    /// potentially, at the same time, filtered by the search query).
    /// @return list of tag names currently being displayed in the left-hand side column.
    std::vector<std::string> displayedSelectedTags() const;

    /// @brief Sets a callback function that will be called whenever search query has
    /// changed in the search bar.
    /// @param callback a function that takes search query text as a parameter.
    void setSearchBarChangedCallback(const SearchBarChangedCallback& callback);

    /// @brief Sets a callback function that will be called whenever the user selects a tag
    /// from the left-hand side column by pressing a specific key on the keyboard (e.g.
    /// 'space-bar').
    /// @param callback a function that takes a tag name as an input parameter.
    void setTagSelectionChangedCallback(const TagSelectionChangedCallback& callback);

    /// @brief Sets a callback function that will be called the user "clicks" the Create
    /// button, i.e. requests to create a new Tag. The search query is passed as a parameter
    /// and may be used as a base for the new Tag name.
    /// @param callback a function that takes the search query text as a parameter.
    void setCreateButtonClickedCallback(const CreateButtonClickedCallback& callback);

    /// @brief A blocking call that displays the picker window. The window closes when the user confirms or
    /// cancels the window.
    /// @return true if the selection has been confirmed, false if the window was canceled.
    bool display();

protected:
    void displayContent() override;

private:
    enum FocusedView { kSearchBar, kTagsList, kSelectedTagsList, kConfirmButton, kCancelButton, kCreateButton, kNone };

    void initializeKeyMap();

    void focusSearchBar();
    void focusTagsList();
    void focusSelectedTagsList();

    bool processSearchBarKey(int key);

    void readTagsListInput();

    void readSelectedTagsListInput();

    void readCreateButtonInput();

    void readConfirmButtonInput();

    void readCancelButtonInput();

    bool confirmCancelWithUser();

    std::shared_ptr<View> viewInFocus() const;

    void onCreateButtonClicked();

    void onConfirmButtonClicked();

    void onCancelButtonClicked();

    bool processKey(int key);

private:
    using KeyFunction = std::function<bool()>;
    friend class QueryChangedListener;
    friend class ViewFocusOperator;

    std::shared_ptr<TextEditView> searchBar_;
    std::shared_ptr<ListView> tagsList_;
    std::shared_ptr<ListView> selectedTagsList_;
    std::shared_ptr<ButtonView> confirmButton_;
    std::shared_ptr<ButtonView> cancelButton_;
    std::shared_ptr<ButtonView> createButton_;
    SearchBarChangedCallback searchQueryChangedCallback_;
    TagSelectionChangedCallback tagSelectionChangedCallback_;
    CreateButtonClickedCallback createButtonClickedCallback_;

    std::map<int, KeyFunction> keyMap_;

    std::unique_ptr<ViewFocusOperator> focusOperator_;

    bool selectionConfirmed_ = false;
};

} // namespace memo::ui
