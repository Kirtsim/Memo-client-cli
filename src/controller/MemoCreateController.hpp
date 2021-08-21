#pragma once
#include "controller/BaseController.hpp"
#include "view/MemoCreateView.hpp"
#include "model/ModelDefs.hpp"

namespace memo::ui {
    class TextEditView;
    class TagPickerView;
    class TagCreateView;
}
namespace memo::ctrl {

class TextEditKeyFilter;


class MemoCreateController : public BaseController<ui::MemoCreateView>
{
    using ResourcesPtr_t = std::shared_ptr<IResources>;
    using TagPickerViewPtr = std::shared_ptr<ui::TagPickerView>;
    using TagCreateViewPtr = std::shared_ptr<ui::TagCreateView>;

public:
    explicit MemoCreateController(const ResourcesPtr_t& resources);
    ~MemoCreateController() override;

    void processInput() override;

private:
    std::vector<model::TagPtr> fetchTags() const;

    void onConfirmNewTagButtonClicked();

    model::TagPtr createTag(const model::TagPtr& tag);

    bool checkMemoTitleAvailability(const std::string& title);

    bool saveMemoDetails();

    bool processKey(int key);

    void stop();

    void pickTags();

    void onTagSearchQueryChanged(const std::string& query,
                                 const std::vector<model::TagPtr>& selectedTags);

    void onTagSelectionChanged(const std::string& tagName, bool selected,
                               std::vector<model::TagPtr>& selectedTags);

    void onCreateTagButtonClicked(const std::string& suggestedTagName);

    void onCreateNewTagNameChanged(const std::string& tagName);

private:
    std::vector<model::TagPtr> tags_;
    std::vector<model::TagPtr> selectedTags_;

    TagPickerViewPtr tagPickerView_;
    TagCreateViewPtr createTagView_;
};

} // namespace memo::ctrl
