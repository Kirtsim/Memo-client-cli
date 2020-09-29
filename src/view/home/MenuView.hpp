#pragma once
#include "view/BaseView.hpp"

#include <string>
#include <vector>
#include <array>

struct tagITEM;
struct tagMENU;

namespace memo {
namespace ui {

class MenuView : public BaseView
{
   static const int MENU_ITEM_COUNT = 6;
   static const std::vector<std::string> kMenuItemNames;

public:
    explicit MenuView(IView* iParent=nullptr);
    explicit MenuView(const Size& iSize, IView* iParent=nullptr);
    MenuView(const Size& iSize, const Position& iPosition, IView* iParent=nullptr);
    ~MenuView();

    bool processInput(unsigned int iInput);
    Size getMinimumRequiredSize() const;
    void positionComponents(Window_t& ioWindow) override;

private:
    std::array<tagITEM*, MENU_ITEM_COUNT + 1> menuItems_;
    WindowPtr_t menuSubWindow_;
    std::unique_ptr<tagMENU> menu_;
};

} // namespace ui
} // namespace memo
