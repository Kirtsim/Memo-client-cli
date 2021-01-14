#include "view/home/HomeView.hpp"
#include "view/ListMemoView.hpp"
#include "Client.hpp"
#include "manager/ViewManager.hpp"
#include <iostream>

namespace memo {
namespace view {

namespace {
    const std::string kPROMPT = "> ";
}

HomeView::HomeView(Client& iClient, const std::shared_ptr<manager::ViewManager>& iViewManager) :
    BaseView(iClient, iViewManager)
{
}

HomeView::~HomeView() = default;

void HomeView::display()
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

bool HomeView::processInput(const std::string& iInput)
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

void HomeView::printMenu() const
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

void HomeView::handleInvalidOption()
{
    printMenu();
    println("Invalid option.");
    print(kPROMPT);
}

} // namespace view
} // namespace memo

#include "view/home/MenuView.hpp"
#include "view/widget/Text.hpp"
#include "view/tools/Tools.hpp"
#include "ncurses/functions.hpp"

namespace memo {
namespace ui {


///////////////////////////////////////////////////////////////
/// HomeView
///////////////////////////////////////////////////////////////
const std::vector<MenuItem> HomeView::kMenuItems {
    MenuItem(E_MenuItem::LIST_MEMOS,  "List Memos", ""),
    MenuItem(E_MenuItem::LIST_TAGS,   "List Tags", ""),
    MenuItem(E_MenuItem::CREATE_MEMO, "Create Memo", ""),
    MenuItem(E_MenuItem::CREATE_TAG,  "Create Tag", ""),
    MenuItem(E_MenuItem::DELETE_MEMO, "Delete Memo", ""),
    MenuItem(E_MenuItem::DELETE_TAG,  "Delete Tag", ""),
    MenuItem(E_MenuItem::EXIT,		  "Exit", ""),
};

HomeView::HomeView(IView* iParent) :
    HomeView( curses::ScreenSize(),
              Position(),
              iParent )
{
}

HomeView::HomeView(const Size& iSize, IView* iParent) :
    HomeView(iSize, Position(), iParent)
{}

HomeView::HomeView(const Size& iSize, const Position& iPosition, IView* iParent) :
    BaseView(iSize, iPosition, iParent),
    errorStatus_(std::make_unique<widget::Text>("SOME TEXT")),
    windowTitle_(std::make_unique<widget::Text>("Welcome to the Memo-client-cli")),
    menuView_(std::make_shared<MenuView>())
{
    registerSubView(menuView_);
    menuView_->setMenuItems(kMenuItems);
    menuView_->setSelectionMark(" * ");
    menuView_->setLayout(Rows(4), Cols(2));
    menuView_->applyMenuChanges();
}

HomeView::~HomeView() = default;

const std::shared_ptr<MenuView>& HomeView::getMenuView()
{
    return menuView_;
}

void HomeView::focus()
{
}

void HomeView::setErrorStatus(const std::string& iStatus)
{
    errorStatus_->setText(iStatus);
}

void HomeView::positionComponents()
{
    windowTitle_->setY(getY() + 2);
    errorStatus_->setY(getY() + getHeight() - 2);
    tools::Tools::centerComponent(*windowTitle_, tools::Center::HORIZONTAL, *this);
    tools::Tools::centerComponent(*errorStatus_, tools::Center::HORIZONTAL, *this);

    Size minMenuSize = menuView_->getMinimumRequiredSize();
    menuView_->setWidth(minMenuSize.width + 4);
    menuView_->setHeight(minMenuSize.height + 4);
    tools::Tools::centerComponent(*menuView_,
                                  tools::Center::HORIZONTAL | tools::Center::VERTICAL,
                                  *this);
}

void HomeView::displayContent()
{
    displayText(*windowTitle_);
    displayText(*errorStatus_);
}

} // namespace ui
} // namespace memo
