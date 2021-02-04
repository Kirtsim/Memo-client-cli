#include "remote/MemoDao.hpp"
#include "remote/ListMemoCall.hpp"
#include "remote/factory/MemoCallFactory.hpp"


namespace memo {
namespace remote {

std::shared_ptr<MemoDaoImpl> MemoDaoImpl::Create(
    std::unique_ptr<MemoCallFactory> callFactory)
{
    return std::make_shared<MemoDaoImpl>(std::move(callFactory));
}

MemoDaoImpl::MemoDaoImpl(std::unique_ptr<MemoCallFactory> callFactory)
    : factory_(std::move(callFactory))
{
}

std::vector<model::Memo> MemoDaoImpl::fetchAll()
{
    auto memoCall = factory_->createMemoSearchCall();
    success_ = false;
    if (!memoCall)
        return {};

    success_ = memoCall->exec();
    std::vector<model::Memo> result;
    if (success_)
    {
        const auto& memos = memoCall->getReply().memos();
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
