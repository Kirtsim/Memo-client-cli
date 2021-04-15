#pragma once
#include "view/BaseView.hpp"
#include <memory>
#include <array>

namespace memo {
namespace ui {

class TextView;
class TextEditView;
class KeyFilter;

class MemoCreateView : public BaseView
{
public:
    static const size_t kSubViewCount = 5;

    enum SubView
    {
        kTitleArea, kDescriptionArea, kTagsArea, kConfirmButton, kCancelButton
    };

public:
    explicit MemoCreateView(IComponent* parent=nullptr);
    explicit MemoCreateView(const Size& size, IComponent* parent=nullptr);
    MemoCreateView(const Size& size, const Position& position, IComponent* parent=nullptr);
    ~MemoCreateView();

    MemoCreateView(const MemoCreateView&) = delete;
    MemoCreateView(const MemoCreateView&&) = delete;
    MemoCreateView& operator=(const MemoCreateView&) = delete;

    void refresh() override;

    void readInput();

    void registerKeyFilter(const std::shared_ptr<KeyFilter>& keyFilter);

    void focusSubView(SubView subView);
    SubView subViewInFocus() const;

    SubView focusNextSubView();
    SubView focusPrevSubView();

protected:
    void displayContent() override;

private:
    std::shared_ptr<TextEditView> memoTitleTextEditView_;
    std::shared_ptr<TextEditView> memoDescriptionTextEditView_;
    std::shared_ptr<TextEditView> memoTagsTextEditView_;

    std::shared_ptr<TextView> cancelButton_;
    std::shared_ptr<TextView> confirmButton_;

    std::shared_ptr<KeyFilter> keyFilter_ = nullptr;
    SubView subViewInFocus_ = SubView::kTitleArea;
    std::array<std::shared_ptr<View>, kSubViewCount> subViewMapping_;
};

} // namespace ui
} // namespace memo
