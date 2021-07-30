#pragma once
#include "view/BaseView.hpp"

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>

namespace memo::ui {

class TextEditView;
class TextView;
class ListView;
class ViewFocusOperator;

using SearchBarChangedCallback = std::function<void(const std::string&)>;
using TagSelectionChangedCallback = std::function<void(const std::string&, bool)>;
using CreateButtonClickedCallback = std::function<void(const std::string&)>;

class TagPickerView : public BaseView
{
public:
    explicit TagPickerView(IComponent* parent=nullptr);

    explicit TagPickerView(const Size& size, IComponent* parent=nullptr);

    TagPickerView(const Size& size, const Position& position, IComponent* parent=nullptr);

    ~TagPickerView() override;

    std::string searchQuery() const;

    void displayTags(const std::vector<std::string>& tagNames);

    std::vector<std::string> displayedTags() const;

    void displaySelectedTagNames(const std::vector<std::string>& tagNames);

    std::vector<std::string> displayedSelectedTags() const;

    void setSearchBarChangedCallback(const SearchBarChangedCallback& callback);

    void setTagSelectionChangedCallback(const TagSelectionChangedCallback& callback);

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

    FocusedView viewInFocus();

private:
    using KeyFunction = std::function<bool()>;
    friend class SearchBarKeyFilter;
    friend class QueryChangedListener;
    friend class ViewFocusOperator;

    std::shared_ptr<TextEditView> searchBar_;
    std::shared_ptr<ListView> tagsList_;
    std::shared_ptr<ListView> selectedTagsList_;
    std::shared_ptr<TextView> confirmButton_;
    std::shared_ptr<TextView> cancelButton_;
    std::shared_ptr<TextView> createButton_;
    SearchBarChangedCallback searchQueryChangedCallback_;
    TagSelectionChangedCallback tagSelectionChangedCallback_;
    CreateButtonClickedCallback createButtonClickedCallback_;


    std::map<int, KeyFunction> keyMap_;

    std::unique_ptr<ViewFocusOperator> focusOperator_;
};

} // namespace memo::ui
