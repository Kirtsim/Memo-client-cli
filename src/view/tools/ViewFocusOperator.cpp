#include "view/tools/ViewFocusOperator.hpp"
#include "view/View.hpp"

namespace memo::ui {

void ViewFocusOperator::stopFocus()
{
    if (auto viewInFocus = this->viewInFocus())
        viewInFocus->looseFocus();
    focusing_ = false;
}

bool ViewFocusOperator::isFocusing() const
{
    return focusing_;
}

std::shared_ptr<View> ViewFocusOperator::viewInFocus() const
{
    if (focusing_ && currentIdx_ < focusables_.size())
        return focusables_[currentIdx_].view;
    return nullptr;
}

std::shared_ptr<View> ViewFocusOperator::selectNext()
{
    auto incrementIndex = [](const SelectFunctionParams& params) {
        return (params.currentIdx + 1) % params.viewCount;
    };
    return selectNew(incrementIndex);
}

std::shared_ptr<View> ViewFocusOperator::selectPrev()
{
    auto decrementIndex = [](const SelectFunctionParams& params) {
        return std::min(params.currentIdx - 1, params.viewCount - 1);
    };
    return selectNew(decrementIndex);
}

void ViewFocusOperator::setFocusables(const std::vector<Focusable>& focusables)
{
    focusables_ = focusables;
    currentIdx_ = 0;
    focusing_ = (!focusables_.empty());
}

std::shared_ptr<View> ViewFocusOperator::selectNew(const std::function<size_t(const SelectFunctionParams&)>& performIndexStep)
{
    if (!focusables_.empty())
    {
        focusables_[currentIdx_].view->looseFocus();
        const auto startIdx = currentIdx_;
        do
        {
            currentIdx_ = performIndexStep({ currentIdx_, focusables_.size() });
        }
        while (currentIdx_ != startIdx && !focusables_[currentIdx_].isFocusable());

        if (focusables_[currentIdx_].isFocusable())
        {
            focusables_[currentIdx_].view->focus();
            return focusables_[currentIdx_].view;
        }

        // At this point none of the views can be focused.
        stopFocus();
    }
    return nullptr;
}

void ViewFocusOperator::resetFocus()
{
    if (focusables_.empty())
        return;
    if (currentIdx_ < focusables_.size())
    {
        focusables_[currentIdx_].view->looseFocus();
    }
    focusing_ = true;
    currentIdx_ = focusables_.size() - 1;
    selectNext();
}
} // namespace memo::ui