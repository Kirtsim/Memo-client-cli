#pragma once
#include "controller/BaseController.hpp"
#include "view/MemoCreateView.hpp"
#include "model/ModelDefs.hpp"

namespace memo::ui {
    class TextEditView;
    class TagPickerView;
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
    bool checkMemoTitleAvailability(const std::string& title);
    bool saveMemoDetails();

private:
    std::vector<model::TagPtr> fetchTags(const std::vector<std::string>& tagNames) const;
    bool processKey(int key);
    void stop();

    void pickTags();

    void onTagSearchQueryChanged(const std::string& query, const TagPickerViewPtr& tagPicker,
                                 const std::vector<model::TagPtr>& selectedTags);

    void onTagSelectionChanged(const std::string& tagName, bool selected,
                               const TagPickerViewPtr& tagPicker,
                               std::vector<model::TagPtr>& selectedTags);

private:
    friend class TextEditKeyFilter;
    std::shared_ptr<TextEditKeyFilter> keyFilter_;
    std::vector<model::TagPtr> tags_;
    std::vector<model::TagPtr> selectedTags_;
    bool run_ = true;
};

} // namespace memo::ctrl
