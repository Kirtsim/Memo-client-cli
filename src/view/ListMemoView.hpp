#pragma once
#include "view/BaseView.hpp"
#include "model/MemoSvc.grpc.pb.h"
#include <string>


namespace memo {
    class Client;
namespace manager {
    class ViewManager;
} // namespace manager
namespace view {

class ListMemoView : public BaseView
{
public:
    static const int kDEF_PAGE_SIZE;

    ListMemoView(Client& client, const std::shared_ptr<manager::ViewManager>& viewManager);
    ListMemoView(const ListMemoView&) = delete;

    ~ListMemoView();
    void display() override;

private:
    void display(const model::MemoSearchRs& response);
    bool processInput(const std::string& input, const model::MemoSearchRs& response);

    bool validateInput(const std::string& input, const model::MemoSearchRs& response) const;

    void printFooter() const;
    void printContent() const;
    void printAfterFooterConent() const;
    void printResponse(const model::MemoSearchRs& response) const;
    void printMemosOnPage(const model::MemoSearchRs& response, int page) const;

    void setButtons(const model::MemoSearchRs& response);
    int getIndexOfFirstOnPage(int page, int totalCount) const;
    int getMemoCountOnPage(int page, int totalCount) const;
    int getPageCount(int memoCount) const;


    bool responseStatusOk_;
    model::MemoSearchRs response_;

    mutable std::string afterFooterContent_;
    int pageSize_;
    int currentPage_;
    bool prevButtonDisplayed_;
    bool nextButtonDisplayed_;
};

} // namespace view
} // namespace memo
