#pragma once
#include "view/BaseView.hpp"
#include <functional>
#include <vector>
#include <map>

namespace memo::ui {
    class TextEditView;
    class TextView;
    class ButtonView;

struct Colour
{
    int red, green, blue;
};

class ViewFocusOperator;
class TagNameChangedListener;

class TagCreateView : public BaseView
{
public:
    explicit TagCreateView(IComponent* parent=nullptr);
    explicit TagCreateView(const Size& size, IComponent* parent=nullptr);
    TagCreateView(const Size& size, const Position& position, IComponent* parent=nullptr);
    ~TagCreateView() override;

    TagCreateView(const TagCreateView&) = delete;
    TagCreateView(const TagCreateView&&) = delete;
    TagCreateView& operator=(const TagCreateView&) = delete;

    void readInput() override;

    void setOnConfirmButtonClicked(const std::function<void(int)>& action);

    void setOnCancelButtonClicked(const std::function<void(int)>& action);

    void setOnTagNameChanged(const std::function<void(const std::string&)>& action);

    void setTagName(const std::string& name);

    const std::string& tagName() const;

    void setColour(const Colour& colour);

    Colour colour() const;

    void setInfoText(const std::string& text);

    void setInfoTextVisible(bool visible);

protected:
    void displayContent() override;

    void onKeyFilterSet(const std::function<bool(int)>& filterFunction) override;

private:
    void initializeKeyMap();

    bool processKey(int key);

private:
    std::shared_ptr<TextEditView> nameEditText_;
    std::shared_ptr<TextView> infoText_;
    std::shared_ptr<ButtonView> confirmButton_;
    std::shared_ptr<ButtonView> cancelButton_;

    std::unique_ptr<ViewFocusOperator> focusOperator_;
    std::shared_ptr<TagNameChangedListener> tagNameChangedListener_;

    std::map<int, std::function<void()>> keyMap_;
};

} // namespace memo::ui
