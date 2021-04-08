#pragma once
#include "view/BaseView.hpp"
#include <memory>

namespace memo {
namespace ui {

class TextView;
class TextEditView;

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

    const std::shared_ptr<TextEditView>& memoTitleTextEditView();
    const std::shared_ptr<TextEditView>& memoDescriptionTextEditView();
    const std::shared_ptr<TextEditView>& memoTagsTextEditView();

protected:
    void displayContent() override;

private:
    std::shared_ptr<TextEditView> memoTitleTextEditView_;
    std::shared_ptr<TextEditView> memoDescriptionTextEditView_;
    std::shared_ptr<TextEditView> memoTagsTextEditView_;
};

} // namespace ui
} // namespace memo
