#include "remote/MemoDao.hpp"
#include "remote/ListMemoCall.hpp"
#include "model/MemoSvc.pb.h"


namespace memo {
namespace remote {

std::shared_ptr<MemoDaoImpl> MemoDaoImpl::Create(
    std::unique_ptr<ListMemoCall> listMemoCall)
{
    return std::make_shared<MemoDaoImpl>(std::move(listMemoCall));
}

MemoDaoImpl::MemoDaoImpl(std::unique_ptr<ListMemoCall> listMemoCall)
    : listMemoCall_(std::move(listMemoCall))
{
}

std::vector<model::Memo> MemoDaoImpl::fetchAll()
{
    success_ = false;
    if (!listMemoCall_)
        return {};

    success_ = listMemoCall_->exec();
    std::vector<model::Memo> result;
    if (success_)
    {
        const auto& memos = listMemoCall_->getReply().memos();
        result.assign(memos.begin(), memos.end());
    }
    return result;
}

bool MemoDaoImpl::success()
{
    return success_;
}

} // namespace remote
} // namespace memo
