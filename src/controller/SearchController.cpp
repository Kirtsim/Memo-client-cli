#include "controller/SearchController.hpp"
#include "ncurses/functions.hpp"
#include "ncurses/keys.hpp"
#include "remote/MemoDao.hpp"

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

namespace {
    std::vector<model::Memo> fetchMemos();
} // naemspace

SearchController::SearchController(const ResourcesPtr_t& resources)
    : BaseController(resources)
{
    auto view = std::make_shared<ui::ListView>();
    setView(view);
    auto memoDao = getResources()->memoDao();
    auto memos = memoDao->fetchAll();
    std::vector<ui::ListItemPtr> memoTitles;
    std::transform(memos.begin(), memos.end(), std::back_inserter(memoTitles),
        [](const model::Memo& memo)
        {
            return std::make_shared<TestListItem>(memo.title());
        });
    view->setItems(memoTitles);
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
        getResources()->controllerManager()->pop();
    }
    curses::KeyPad(window, DISABLE);
}

} // namespace ctrl
} // namespace memo
