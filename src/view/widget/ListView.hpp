#pragma once
#include "view/BaseView.hpp"

#include <vector>

namespace memo {
namespace ui {

class ListItem
{
public:
    ~ListItem() = default;

    virtual std::string text() const = 0;
};

using ListItemPtr = std::shared_ptr<ListItem>;

class ListView : public BaseView
{
public:
    explicit ListView(IComponent* parent=nullptr);
    explicit ListView(const Size& size, IComponent* parent=nullptr);
    ListView(const Size& size, const Position& position, IComponent* parent=nullptr);

    void setItems(const std::vector<ListItemPtr>& items);

    const std::vector<ListItemPtr>& items() const;

    size_t itemCount() const;

    void select(size_t pos);

    bool selectNext();

    bool selectPrev();

    ListItemPtr selected() const;

    size_t selectedPos() const;
protected:
    void displayContent() override;

private:
    size_t maxVisibleItems() const;

private:    
    std::vector<ListItemPtr> items_;

    size_t firstVisItem_ = 0;
    size_t lastVisItem_ = 0;
    size_t selected_ = 0;
};

} // namespace ui
} // namespace memo
