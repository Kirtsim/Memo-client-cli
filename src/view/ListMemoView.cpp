#include "view/ListMemoView.hpp"
#include "remote/ListMemoCall.hpp"
#include "manager/ViewManager.hpp"
#include "Client.hpp"

#include <iostream>
#include <sstream>


namespace memo {
namespace view {

const int ListMemoView::kDEF_PAGE_SIZE = 5;

namespace {
const std::string kHEADER =
        "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
        "+                     Memo list\n"
        "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";

const std::string kPROMPT = "> ";

const std::string kBUTTON_PREVIOUS = "[P] previous    ";
const std::string kBUTTON_NEXT     = "[N] next    ";
const std::string kBUTTON_CLOSE    = "[C] close";
const std::string kINVALID_OPTION  = "Invalid option.";

const bool kCONTINUE = true;
const bool kSTOP     = false;

} // namespace

ListMemoView::ListMemoView(Client& iClient, const std::shared_ptr<manager::ViewManager>& iViewManager) :
    BaseView(iClient, iViewManager),
    responseStatusOk_(false),
    pageSize_(kDEF_PAGE_SIZE),
    currentPage_(0),
    prevButtonDisplayed_(false),
    nextButtonDisplayed_(false)
{}

ListMemoView::~ListMemoView() = default;

void ListMemoView::display()
{
    println(kHEADER);
    println("Fetching memos from server ...");
    remote::ListMemoCall remoteCall(getClient().getMemoStub());
    responseStatusOk_ = remoteCall.exec();

    response_ = remoteCall.getReply();
    bool active = true;
    while (active)
    {
        display(response_);
        std::string input = readInput();
        active = processInput(input, response_);
    }
}

bool ListMemoView::processInput(const std::string& iInput, const model::MemoSearchRs& iResponse)
{
    if (!validateInput(iInput, iResponse))
        return kCONTINUE;

    const char choice = iInput[0];
    switch (choice)
    {
        case '1':
            afterFooterContent_ = "Option 1. Memo view not yet implemented.";
            break;
        case '2':
            afterFooterContent_ = "Option 2. Memo view not yet implemented.";
            break;
        case '3':
            afterFooterContent_ = "Option 3. Memo view not yet implemented.";
            break;
        case '4':
            afterFooterContent_ = "Option 4. Memo view not yet implemented.";
            break;
        case '5':
            afterFooterContent_ = "Option 5. Memo view not yet implemented.";
            break;
        case 'P': case 'p':
            currentPage_++;
            break;
        case 'N': case 'n':
            currentPage_--;
            break;
        case 'C': case 'c':
            getViewManager()->popView();
            return kSTOP;
        default:
            afterFooterContent_ = kINVALID_OPTION;
    }
    return kCONTINUE;
}

bool ListMemoView::validateInput(const std::string& iInput, const model::MemoSearchRs& iResponse) const
{
    if (iInput.size() > 1 || iInput.empty())
    {
        afterFooterContent_ = kINVALID_OPTION;
        return false;
    }

    const char choice = iInput[0];
    if (std::isdigit(choice))
    {
        const int numInput  = choice - '0';
        const int memoCount = getMemoCountOnPage(currentPage_, iResponse.memos().size());
        if (numInput <= memoCount)
            return true;
    }
    else if ((choice == 'P' || choice == 'p') && prevButtonDisplayed_)
        return true;
    else if ((choice == 'N' || choice == 'n') && nextButtonDisplayed_)
        return true;
    else if  (choice == 'C' || choice == 'c')
        return true;
    afterFooterContent_ = kINVALID_OPTION;
    return false;
}

void ListMemoView::display(const model::MemoSearchRs& iResponse)
{
    println(kHEADER);
    printContent();
    setButtons(iResponse);
    printFooter();
    printAfterFooterConent();
    print(kPROMPT);
}

void ListMemoView::printFooter() const
{
    println("------------------------------------------------------------");
    if (prevButtonDisplayed_)
        print(kBUTTON_PREVIOUS);

    if (nextButtonDisplayed_)
        print(kBUTTON_NEXT);

    println(kBUTTON_CLOSE);
    println("------------------------------------------------------------");
}

void ListMemoView::printContent() const
{
    if (!responseStatusOk_)
        println("Remote call failed :(");
    else
        printResponse(response_);
}

void ListMemoView::printAfterFooterConent() const
{
    if (!afterFooterContent_.empty())
    {
        println(afterFooterContent_);
        afterFooterContent_ = "";
    }
}

void ListMemoView::printResponse(const model::MemoSearchRs& iResponse) const
{
    if (iResponse.memos().empty())
    {
        println("No memos found :(");
        return;
    }
    printMemosOnPage(iResponse, 0);
}

void ListMemoView::printMemosOnPage(const model::MemoSearchRs& iResponse, int iPage) const
{
    const auto& memos = iResponse.memos();
    const int memoCount = memos.size();

    const int startIndex = getIndexOfFirstOnPage(iPage, memoCount);
    const int endIndex = std::min(memoCount, startIndex + pageSize_);
    std::stringstream sstream;
    for (int i = startIndex; i < endIndex; ++i)
    {
        sstream << "[" << (i+1) << "] " << memos[i].title() << "\n";
    }
    sstream << "\n";
    sstream << "page " << (currentPage_+1) << "/" << getPageCount(memoCount);

    println(sstream.str());
}

void ListMemoView::setButtons(const model::MemoSearchRs& iResponse)
{
    const auto memos = iResponse.memos();
    const int indexOfFirst = getIndexOfFirstOnPage(currentPage_, memos.size());
    const int endIndex = std::min(memos.size(), indexOfFirst + pageSize_);
    prevButtonDisplayed_ = indexOfFirst > 0;
    nextButtonDisplayed_ = endIndex < memos.size();
}

int ListMemoView::getIndexOfFirstOnPage(const int iPage, const int iTotalCount) const
{
    return (pageSize_ * iPage) % iTotalCount;
}

int ListMemoView::getMemoCountOnPage(const int iPage, const int iTotalCount) const
{
    const int indexOfFirst = getIndexOfFirstOnPage(iPage, iTotalCount);
    const int endIndex = std::min(iTotalCount, indexOfFirst + pageSize_);
    return endIndex - indexOfFirst;
}

int ListMemoView::getPageCount(int memoCount) const
{
    if (pageSize_ == 0)
        return 0;
    return memoCount / pageSize_ + 1;
}

} // namespace view
} // namespace memo
