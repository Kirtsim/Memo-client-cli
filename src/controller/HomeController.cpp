#include "controller/HomeController.hpp"
#include "view/home/HomeView.hpp"
#include "view/home/MenuView.hpp"
#include <ncursesw/menu.h>

namespace memo {
namespace ctrl {

HomeController::HomeController(const ResourcesPtr_t& iResources) :
    BaseController(iResources)
{
    auto view = std::make_shared<ui::HomeView>();
    setView(view);
}

void HomeController::processInput()
{
    auto& cursesWindow = view()->getWindow();
    keypad(&cursesWindow, TRUE);
    unsigned int input = wgetch(&cursesWindow);
    switch (input)
    {
        case KEY_DOWN:
            view()->getMenuView()->navigateMenuDown(); break;
        case KEY_UP:
            view()->getMenuView()->navigateMenuUp(); break;
        case KEY_LEFT:
            view()->getMenuView()->navigateMenuLeft(); break;
        case KEY_RIGHT:
            view()->getMenuView()->navigateMenuRight(); break;
            break;
        case KEY_ENTER:
            int selectedItem = view()->getMenuView()->getSelected();
            onMenuOptionSelected(selectedItem);
            break;
    }
    keypad(&cursesWindow, FALSE);
}


void HomeController::onMenuOptionSelected(int iSelectedOption)
{
    
}

} // namespace ctrl
} // namespace memo
