#pragma once
#include "view/BaseView.hpp"
#include <memory>
#include <vector>
#include <array>
#include <functional>

namespace memo::ui {

class TextView;
class TextEditView;
class ButtonView;
class KeyFilter;

class MemoCreateView : public BaseView
{
public:
    static const size_t kSubViewCount = 5;

    enum SubView
    {
        kTitleArea, kDescriptionArea, kTagsArea, kConfirmButton, kCancelButton, kNone
    };

public:
    explicit MemoCreateView(IComponent* parent=nullptr);
    explicit MemoCreateView(const Size& size, IComponent* parent=nullptr);
    MemoCreateView(const Size& size, const Position& position, IComponent* parent=nullptr);
    ~MemoCreateView() override;

    MemoCreateView(const MemoCreateView&) = delete;
    MemoCreateView(const MemoCreateView&&) = delete;
    MemoCreateView& operator=(const MemoCreateView&) = delete;

    void readInput() override;

    void focusSubView(SubView subView);
    SubView subViewInFocus() const;

    SubView focusNextSubView();
    SubView focusPrevSubView();

    const std::string& memoTitle() const;

    const std::string& memoDescription() const;

    void displayTagNames(const std::vector<std::string>& tagNames);

    void doOnConfirmButtonClicked(const std::function<void()>& functionCall);

    void doOnCancelButtonClicked(const std::function<void()>& functionCall);

protected:
    void displayContent() override;

    void onKeyFilterSet(const std::function<bool(int)>& filterFunction) override;

private:
    void layoutComponents();

private:
    std::shared_ptr<TextEditView> memoTitleTextEditView_;
    std::shared_ptr<TextEditView> memoDescriptionTextEditView_;
    std::shared_ptr<TextView> tags_;
    std::shared_ptr<TextView> tagsAreaHintLabel_;

    std::shared_ptr<ButtonView> cancelButton_;
    std::shared_ptr<ButtonView> confirmButton_;

    std::shared_ptr<KeyFilter> keyFilter_ = nullptr;
    SubView subViewInFocus_ = kNone;
    std::array<std::shared_ptr<View>, kSubViewCount> subViewMapping_;

    std::function<void()> onConfirmButtonClicked_;
    std::function<void()> onCancelButtonClicked_;
};

} // namespace memo::ui
