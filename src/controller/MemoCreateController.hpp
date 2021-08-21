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

public:
    explicit MemoCreateController(const ResourcesPtr_t& resources);
    ~MemoCreateController() override;

    void processInput() override;

private:
    std::vector<model::TagPtr> fetchTags() const;

    void onConfirmNewTagButtonClicked(ui::TagCreateView& tagCreateView);

    model::TagPtr createTag(const model::TagPtr& tag);

    bool checkMemoTitleAvailability(const std::string& title);

    bool saveMemoDetails();

    bool processKey(int key);

    void stop();

    void pickTags();

    void onTagSearchQueryChanged(const std::string& query, const TagPickerViewPtr& tagPicker,
                                 const std::vector<model::TagPtr>& selectedTags);

    void onTagSelectionChanged(const std::string& tagName, bool selected,
                               const TagPickerViewPtr& tagPicker,
                               std::vector<model::TagPtr>& selectedTags);

    void onCreateTagButtonClicked(const std::string& suggestedTagName, ui::TagPickerView& tagPicker);

    void onCreateNewTagNameChanged(const std::string& tagName, ui::TagCreateView& tagCreateView);

private:
    std::vector<model::TagPtr> tags_;
    std::vector<model::TagPtr> selectedTags_;
};

} // namespace memo::ctrl
