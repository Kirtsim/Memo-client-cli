#pragma once
#include "view/BaseView.hpp"

#include <memory>

namespace memo {
namespace ui {

class ListView;
//class TextView;

using ListViewPtr = std::shared_ptr<ListView>;

class MemoSearchView : public BaseView
{
public:
    explicit MemoSearchView(IComponent* parent=nullptr);
    explicit MemoSearchView(const Size& size, IComponent* parent=nullptr);
    MemoSearchView(const Size& size, const Position& position, IComponent* parent=nullptr);

    const ListViewPtr& memoListView();

    void refresh() override;

protected:
    void displayContent() override;

private:
    ListViewPtr memoListView_;
//    std::shared_ptr<TextView> navigateBackText_; 
};

} // namespace ui
} // namespace memo
