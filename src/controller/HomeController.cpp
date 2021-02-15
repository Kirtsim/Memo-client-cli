#include "controller/HomeController.hpp"
#include "controller/SearchController.hpp"
#include "controller/MemoCreateController.hpp"
#include "view/home/HomeView.hpp"
#include "view/home/MenuView.hpp"
#include "manager/ControllerManager.hpp"

#include "ncurses/functions.hpp"
#include "ncurses/keys.hpp"

namespace memo {
namespace ctrl {

HomeController::HomeController(const ResourcesPtr_t& resources) :
    BaseController(resources)
{
    auto view = std::make_shared<ui::HomeView>();
    setView(view);
}

void HomeController::processInput()
{
    auto& cursesWindow = view()->getMenuView()->getWindow();

    curses::KeyPad(cursesWindow, ENABLE);
    const int input = curses::ReadChar(cursesWindow);
    if (input == curses::Key::kDown)
    {
        view()->getMenuView()->navigateMenuDown();
    }
    else if (input == curses::Key::kUp)
    {
        view()->getMenuView()->navigateMenuUp();
    }
    else if (input == curses::Key::kLeft)
    {
        view()->getMenuView()->navigateMenuLeft();
    }
    else if (input == curses::Key::kRight)
    {
        view()->getMenuView()->navigateMenuRight();
    }
    else if (input == curses::Key::kEnter)
    {
        const auto& selection = view()->getMenuView()->getSelected();
        onMenuOptionSelected(selection);
    }
    else if (input == 'q')
    {
        getResources()->controllerManager()->pop();
    }
    else
    {
        view()->setErrorStatus("Unprocessed Key code: " + std::to_string(input));
    }
    curses::KeyPad(DISABLE);
}


void HomeController::onMenuOptionSelected(std::pair<bool, ui::MenuItem> selectedOption)
{
    const auto& home_view = view();
    if (!selectedOption.first)
    {
        home_view->setErrorStatus("Nothing selected");
        return;
    }

    const auto& menuItem  = selectedOption.second;
    switch (menuItem.getId())
    {
        case ui::E_MenuItem::CREATE_MEMO:
        {
            auto manager = getResources()->controllerManager();
            manager->add(std::make_shared<MemoCreateController>(getResources()));
            home_view->refreshOnRequest();
        } break;
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
        {
            auto manager = getResources()->controllerManager();
            manager->add(std::make_shared<SearchController>(getResources()));
            home_view->refreshOnRequest();
        } break;
        case ui::E_MenuItem::LIST_TAGS:
            home_view->setErrorStatus(menuItem.getName());
            break;
        case ui::E_MenuItem::EXIT:
            getResources()->controllerManager()->pop();
            break;
        default:
            home_view->setErrorStatus("Unknown option");
    }
}

} // namespace ctrl
} // namespace memo
