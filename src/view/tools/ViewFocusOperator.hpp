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

    /// @brief Resets the focus and places it back on the first view.
    /// Can be used in case the the ViewFocusOperator no longer focuses
    /// any view or when there is a view already in focus. If the very
    /// first view is already in focus, this method has no effect.
    /// Similarly, if there are no views to focus, or none of the views
    /// is focusable this method has no effect either.
    void resetFocus();

private:
    struct SelectFunctionParams { size_t currentIdx; size_t viewCount; };
    std::shared_ptr<View> selectNew(const std::function<size_t(const SelectFunctionParams&)>& );

private:
    size_t currentIdx_ = 0;
    bool focusing_ = false;
    std::vector<Focusable> focusables_;
};

} // namespace memo::ui
