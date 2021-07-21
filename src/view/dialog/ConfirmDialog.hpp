#pragma once
#include "view/BaseView.hpp"
#include "view/widget/TextView.hpp"
#include <string>

namespace memo::ui {

class ConfirmDialog : public BaseView
{
public:
    static const int kMaxWidth = 400;

    static void Display(const std::string& message, IComponent* parent=nullptr);

    explicit ConfirmDialog(IComponent* parent=nullptr);
    explicit ConfirmDialog(const Size& size, IComponent* parent=nullptr);
    ConfirmDialog(const Size& size, const Position& position, IComponent* parent=nullptr);
    ~ConfirmDialog() override;

    ConfirmDialog(const ConfirmDialog&) = delete;
    ConfirmDialog(const ConfirmDialog&&) = delete;
    ConfirmDialog& operator=(const ConfirmDialog&) = delete;

    void setMessage(const std::string& message);
    const std::string& message() const;

    /// @brief A blocking call which displays the dialog and waits for the user to confirm
    /// by hitting the <Enter> or <Esc> keys.
    void display();

protected:
    void onPositionChanged(const Position& oldPos, const Position& newPos) override;

    void positionComponents() override;
    void beforeViewResized() override;

private:
    void updateComponentsPosition();

private:
    static const int kPaddingVertical = 1;
    static const int kPaddingHorizontal = 2;

    std::shared_ptr<TextView> message_;
    std::shared_ptr<TextView> okButton_;
};

} // namespace memo::ui
