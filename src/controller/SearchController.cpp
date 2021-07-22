#include "controller/SearchController.hpp"
#include "ncurses/functions.hpp"
#include "ncurses/keys.hpp"
#include "remote/MemoService.hpp"
#include "remote/ListMemoRequest.hpp"
#include "remote/ServiceResponse.hpp"
#include "remote/ListMemoResponseData.hpp"
#include "remote/ServiceEnums.hpp"
#include "view/widget/ListView.hpp"


namespace memo::ctrl {

class TestListItem : public ui::ListItem
{
public:
    explicit TestListItem(const std::string& text)
        : text_(text)
    {
    }

    ~TestListItem() override = default;

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
    auto view = std::make_shared<ui::MemoSearchView>(curses::ScreenSize());
    setView(view);
    fetchMemos();
}


void SearchController::fetchMemos()
{
    std::vector<ui::ListItemPtr> memoTitles;
    auto memoService = getResources()->memoService();
    if (!memoService)
    {
        // TODO: log error + display message
        return;
    }
    remote::ListMemoRequestBuilder requestBuilder;
    requestBuilder.setRequestId("0000-1111-2222-3333-4444") // TODO: Generate a unique uuid
            .setMemoView(remote::ModelView::kMinimalView)
            .setPageSize(20);
    auto response = memoService->listMemos(requestBuilder.build());
    if (response && response->status() == remote::ResponseStatus::kSuccess)
    {
        for (const auto& memo : response->data().memos())
        {
            if (memo)
                memoTitles.emplace_back(std::make_shared<TestListItem>(memo->title()));
        }
    }
    else
    {
        //TODO: log an error + show the user a message that sth went wrong
    }

    if (auto rootView = view())
    {
        rootView->memoListView()->setItems(memoTitles);
    }
}

void SearchController::processInput()
{
    if (!view())
        return;
    auto& window = view()->memoListView()->getWindow();

    curses::KeyPad(window, ENABLE);
    const int input = curses::ReadChar(window);

    if (input == curses::Key::kDown)
    {
        view()->memoListView()->selectNext();
    } 
    else if (input == curses::Key::kUp)
    {
        view()->memoListView()->selectPrev();
    }
    else if (input == 'q')
    {
        getResources()->controllerManager()->pop();
    }
    curses::KeyPad(window, DISABLE);
}

} // namespace memo::ctrl
