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

    ListMemoView(Client& iClient, const std::shared_ptr<manager::ViewManager>& iViewManager);
    ListMemoView(const ListMemoView&) = delete;

    ~ListMemoView();
    void display() override;

private:
    void display(const model::MemoSearchRs& iResponse);
    bool processInput(const std::string& iInput, const model::MemoSearchRs& iResponse);

    bool validateInput(const std::string& iInput, const model::MemoSearchRs& iResponse) const;

    void printFooter() const;
    void printContent() const;
    void printAfterFooterConent() const;
    void printResponse(const model::MemoSearchRs& iResponse) const;
    void printMemosOnPage(const model::MemoSearchRs& iResponse, int iPage) const;

    void setButtons(const model::MemoSearchRs& iResponse);
    int getIndexOfFirstOnPage(int iPage, int iTotalCount) const;
    int getMemoCountOnPage(int iPage, int iTotalCount) const;


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
