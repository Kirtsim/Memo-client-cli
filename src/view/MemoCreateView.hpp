#pragma once
#include "view/BaseView.hpp"
#include <memory>

namespace memo {
namespace ui {

class TextView;

class MemoCreateView : public BaseView
{
public:
    explicit MemoCreateView(IComponent* parent=nullptr);
    explicit MemoCreateView(const Size& size, IComponent* parent=nullptr);
    MemoCreateView(const Size& size, const Position& position, IComponent* parent=nullptr);
    ~MemoCreateView();

    MemoCreateView(const MemoCreateView&) = delete;
    MemoCreateView(const MemoCreateView&&) = delete;
    MemoCreateView& operator=(const MemoCreateView&) = delete;

    void refresh() override;

    const std::shared_ptr<TextView>& memoTitleTextView();
    const std::shared_ptr<TextView>& memoDescriptioneTextView();
    const std::shared_ptr<TextView>& memoTagsTextView();

protected:
    void displayContent() override;

private:
    std::shared_ptr<TextView> memoTitleText_;
    std::shared_ptr<TextView> memoDescriptionText_;
    std::shared_ptr<TextView> memoTagsText_;

};

} // namespace ui
} // namespace memo
