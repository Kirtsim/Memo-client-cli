#include "view/MenuView.hpp"
#include "view/ListMemoView.hpp"
#include "Client.hpp"
#include "manager/ViewManager.hpp"
#include <iostream>

namespace memo {
namespace view {

namespace {
    const std::string kPROMPT = "> ";
}

MenuView::MenuView(Client& iClient, const std::shared_ptr<manager::ViewManager>& iViewManager) :
    BaseView(iClient, iViewManager)
{
}

MenuView::~MenuView() = default;

void MenuView::display()
{
    printMenu();
    print(kPROMPT);
    bool active = true;
    while (active)
    {
        const auto input = readInput();
        active = processInput(input);
    }
}

bool MenuView::processInput(const std::string& iInput)
{
    if (iInput.size() > 1)
    {
        handleInvalidOption();
        return true;
    }
    const char choice = iInput[0];
    const bool yesCacheThisView = true;
    switch (choice)
    {
        case 'x': case 'X':
           std::exit(0);
        case '1':
        {
           auto view = std::make_shared<ListMemoView>(getClient(), getViewManager());
           getViewManager()->addView(view, yesCacheThisView);
           return false;
        }
        case '2':
           printMenu();
           println("Option 2. Not yet implemented.");
           print(kPROMPT);
           break;
        case '3':
           printMenu();
           println("Option 3. Not yet implemented.");
           print(kPROMPT);
           break;
        case '4':
           printMenu();
           println("Option 4. Not yet implemented.");
           print(kPROMPT);
           break;
        case '5':
           printMenu();
           println("Option 5. Not yet implemented.");
           print(kPROMPT);
           break;
        case '6':
           printMenu();
           println("Option 6. Not yet implemented.");
           print(kPROMPT);
           break;
        default:
           handleInvalidOption();
    }
    return true;
}

void MenuView::printMenu() const
{
    std::string options =
        "############################################################\n"
        "#               ! Welcome to MEMO CLIENT !\n"
        "############################################################\n"
        "\n"
        "List:    [1] Memos   [2] Tags\n"
        "Search:  [3] Memo    [4] Tag\n"
        "Create:  [5] Memo    [6] Tag\n"
        "\n"
        "------------------------------------------------------------\n"
        "[X] Exit\n"
        "------------------------------------------------------------";
    println(options);
}

void MenuView::handleInvalidOption()
{
    printMenu();
    println("Invalid option.");
    print(kPROMPT);
}

} // namespace view
} // namespace memo

#include "view/widget/Text.hpp"
#include "view/tools/Tools.hpp"
#include <ncursesw/curses.h>

namespace memo {
namespace ui {

MenuView::MenuView(const IView::Ptr& iParent) :
    MenuView({ LINES, COLS }, { 0, 0 }, iParent)
{}

MenuView::MenuView(const Size& iSize, const IView::Ptr& iParent) :
    MenuView(iSize, { 0, 0 }, iParent)
{}

MenuView::MenuView(const Size& iSize, const Position& iPosition, const IView::Ptr& iParent) :
    BaseView(iSize, iPosition, iParent)
{}

MenuView::~MenuView() = default;

void MenuView::focus()
{

}

void MenuView::populateWindow(Window_t& ioWindow)
{
    widget::Text title("| Welcome to the Memo-client-cli |\n");
    tools::Tools::centerComponent(title, tools::CenterType::HORIZONTAL, *this);
    title.setY(1);
    displayText(title);
}

} // namespace ui
} // namespace memo


