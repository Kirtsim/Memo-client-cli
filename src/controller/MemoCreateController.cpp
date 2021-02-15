#include "controller/MemoCreateController.hpp"
#include "view/widget/TextView.hpp"
#include "manager/ControllerManager.hpp"

#include "ncurses/functions.hpp"
#include "ncurses/keys.hpp"

namespace memo {
namespace ctrl {

MemoCreateController::MemoCreateController(const ResourcesPtr_t& resources) :
    BaseController(resources)
{
    auto view = std::make_shared<ui::MemoCreateView>();
    setView(view);
}

void MemoCreateController::processInput()
{
    auto& cursesWindow = view()->memoTitleTextView()->getWindow();

    curses::KeyPad(cursesWindow, ENABLE);
    const int input = curses::ReadChar(cursesWindow);
    if (input == curses::Key::kDown)
    {

    }
    else if (input == curses::Key::kUp)
    {

    }
    else if (input == curses::Key::kLeft)
    {

    }
    else if (input == curses::Key::kRight)
    {

    }
    else if (input == curses::Key::kEnter)
    {
//        const auto& selection = view()->getMenuView()->getSelected();
//        onMenuOptionSelected(selection);
    }
    else if (input == 'q')
    {
        getResources()->controllerManager()->pop();
    }
    curses::KeyPad(DISABLE);
}

} // namespace ctrl
} // namespace memo

