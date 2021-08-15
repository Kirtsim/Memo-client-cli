#pragma once
#include <memory>
#include <vector>

namespace memo::ui {

class View;

class ViewFocusOperator
{
public:
    struct Focusable
    {
        std::shared_ptr<View> view;
        std::function<bool()> isFocusable;
    };

    void stopFocus();

    bool isFocusing() const;

    std::shared_ptr<View> viewInFocus() const;

    std::shared_ptr<View> selectNext();

    std::shared_ptr<View> selectPrev();

    void setFocusables(const std::vector<Focusable>& focusables);

private:
    struct SelectFunctionParams { size_t currentIdx; size_t viewCount; };
    std::shared_ptr<View> selectNew(const std::function<size_t(const SelectFunctionParams&)>& );

private:
    size_t currentIdx_ = 0;
    bool focusing_ = false;
    std::vector<Focusable> focusables_;
};

} // namespace memo::ui
