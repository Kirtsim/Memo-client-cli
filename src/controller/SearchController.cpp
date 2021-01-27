#include "controller/SearchController.hpp"
#include "ncurses/functions.hpp"
#include "ncurses/keys.hpp"
#include "view/widget/Text.hpp"

namespace memo {
namespace ctrl {

class TestListItem : public ui::ListItem
{
public:
    TestListItem(const std::string& text)
        : text_(text)
    {
    }

    std::string text() const override
    {
        return text_;
    }

private:
    std::string text_;
};

SearchController::SearchController(const ResourcesPtr_t& resources)
    : BaseController(resources)
{
    auto view = std::make_shared<ui::ListView>();
    std::vector<ui::ListItemPtr> items {
        std::make_shared<TestListItem>("This is my first memo."),
        std::make_shared<TestListItem>("This is a second memo title."),
        std::make_shared<TestListItem>("Very long title that will likely be troublesome to display, especially on narrower screens."),
        std::make_shared<TestListItem>("Short title"),
        std::make_shared<TestListItem>("1"),
        std::make_shared<TestListItem>("2"),
        std::make_shared<TestListItem>("3"),
        std::make_shared<TestListItem>("4"),
        std::make_shared<TestListItem>("5"),
        std::make_shared<TestListItem>("6"),
        std::make_shared<TestListItem>("7"),
        std::make_shared<TestListItem>("8"),
        std::make_shared<TestListItem>("9"),
        std::make_shared<TestListItem>("10"),
        std::make_shared<TestListItem>("11"),
        std::make_shared<TestListItem>("12"),
        std::make_shared<TestListItem>("13"),
        std::make_shared<TestListItem>("14"),
        std::make_shared<TestListItem>("15"),
        std::make_shared<TestListItem>("16"),
        std::make_shared<TestListItem>("17"),
        std::make_shared<TestListItem>("18"),
        std::make_shared<TestListItem>("19"),
        std::make_shared<TestListItem>("20"),
        std::make_shared<TestListItem>("21"),
        std::make_shared<TestListItem>("22"),
        std::make_shared<TestListItem>("23"),
        std::make_shared<TestListItem>("24"),
        std::make_shared<TestListItem>("25"),
        std::make_shared<TestListItem>("26"),
        std::make_shared<TestListItem>("27"),
        std::make_shared<TestListItem>("28"),
        std::make_shared<TestListItem>("29"),
        std::make_shared<TestListItem>("30"),
        std::make_shared<TestListItem>("31"),
        std::make_shared<TestListItem>("32"),
        std::make_shared<TestListItem>("33"),
        std::make_shared<TestListItem>("34"),
        std::make_shared<TestListItem>("35"),
        std::make_shared<TestListItem>("36"),
        std::make_shared<TestListItem>("37"),
        std::make_shared<TestListItem>("38"),
        std::make_shared<TestListItem>("39"),
        std::make_shared<TestListItem>("40"),

    };
    view->setItems(items);
    setView(view);
}

void SearchController::processInput()
{
    if (!view())
        return;
    auto& window = view()->getWindow();

    curses::KeyPad(window, ENABLE);
    const int input = curses::ReadChar(window);

    if (input == curses::Key::kDown)
    {
        view()->selectNext();
    } 
    else if (input == curses::Key::kUp)
    {
        view()->selectPrev();
    }
    else if (input == 'q')
    {
        getResources()->getControllerManager()->pop();
    }
    curses::KeyPad(window, DISABLE);
}

} // namespace ctrl
} // namespace memo
