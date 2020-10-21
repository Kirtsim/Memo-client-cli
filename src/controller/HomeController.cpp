#include "controller/HomeController.hpp"
#include "view/home/HomeView.hpp"
#include "view/home/MenuView.hpp"
#include "manager/ControllerManager.hpp"
#include <ncursesw/menu.h>

namespace memo {
namespace ctrl {

const unsigned int kKeyEnter = 10;

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
    int input = wgetch(&cursesWindow);

    switch (input)
    {
        case KEY_DOWN:
            view()->getMenuView()->navigateMenuDown();
            break;
        case KEY_UP:
            view()->getMenuView()->navigateMenuUp();
            break;
        case KEY_LEFT:
            view()->getMenuView()->navigateMenuLeft();
            break;
        case KEY_RIGHT:
            view()->getMenuView()->navigateMenuRight();
            break;
        case kKeyEnter:
        {
            const auto& selection = view()->getMenuView()->getSelected();
            onMenuOptionSelected(selection);
        } break;
        default:
            view()->setErrorStatus("Unprocessed Key code: " + std::to_string(input));
    }
    keypad(&cursesWindow, FALSE);
}


void HomeController::onMenuOptionSelected(std::pair<bool, ui::MenuItem> iSelectedOption)
{
    const auto& home_view = view();
    if (!iSelectedOption.first)
    {
        home_view->setErrorStatus("Nothing selected");
        return;
    }

    const auto& menuItem  = iSelectedOption.second;
    switch (menuItem.getId())
    {
        case ui::E_MenuItem::CREATE_MEMO:
            home_view->setErrorStatus(menuItem.getName());
            break;
        case ui::E_MenuItem::CREATE_TAG:
            home_view->setErrorStatus(menuItem.getName());
            break;
        case ui::E_MenuItem::DELETE_MEMO:
            home_view->setErrorStatus(menuItem.getName());
            break;
        case ui::E_MenuItem::DELETE_TAG:
            home_view->setErrorStatus(menuItem.getName());
            break;
        case ui::E_MenuItem::LIST_MEMOS:
            home_view->setErrorStatus(menuItem.getName());
            break;
        case ui::E_MenuItem::LIST_TAGS:
            home_view->setErrorStatus(menuItem.getName());
            break;
        case ui::E_MenuItem::EXIT:
            getResources()->getControllerManager()->pop();
            break;
        default:
            home_view->setErrorStatus("Unknown option");
    }
}

} // namespace ctrl
} // namespace memo
