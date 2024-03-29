#pragma once
#include "view/BaseView.hpp"

#include <string>
#include <vector>

namespace memo::ui {

class ListItem
{
public:
    virtual ~ListItem() = default;

    virtual std::string text() const = 0;
};

using ListItemPtr = std::shared_ptr<ListItem>;

class ListView : public BaseView
{
public:
    explicit ListView(IComponent* parent=nullptr);
    explicit ListView(const Size& size, IComponent* parent=nullptr);
    ListView(const Size& size, const Position& position, IComponent* parent=nullptr);

    ~ListView() override;

    void setItems(const std::vector<ListItemPtr>& items);

    const std::vector<ListItemPtr>& items() const;

    std::vector<ListItemPtr> items();

    size_t itemCount() const;

    bool empty() const;

    void select(size_t pos);

    bool selectNext();

    bool selectPrev();

    void setSelectionMark(const std::string& mark);

    const std::string& selectionMark() const;

    ListItemPtr selected() const;

    size_t selectedPos() const;

protected:
    void displayContent() override;

private:
    size_t maxVisibleItems() const;

    size_t maxRowChars() const;

    void displayTextRows(const std::vector<std::string>& items);

    void clearDisplayedRows();

private:    
    std::vector<ListItemPtr> items_;
    std::string selectionMark_ = "* ";

    size_t firstVisItem_ = 0;
    size_t lastVisItem_ = 0;
    size_t selected_ = 0;
};

} // namespace memo::ui
