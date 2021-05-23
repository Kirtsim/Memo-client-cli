#include "remote/MemoDao.hpp"
#include "remote/ListMemoCall.hpp"
#include "remote/MemoCreateCall.hpp"
#include "remote/factory/MemoCallFactory.hpp"


namespace memo::remote {

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

model::MemoCreateRs MemoDaoImpl::add(const model::Memo& memo)
{
    auto memoCall = factory_->createMemoCreateCall();
    success_ = false;
    model::MemoCreateRs result;
    result.mutable_operationstatus()->set_status(model::OperationStatus::FAILURE);
    if (!memoCall)
        return result;

    memoCall->setMemo(memo);
    if ((success_ = memoCall->exec()))
    {
        result = memoCall->getReply();
    }
    return result;
}

bool MemoDaoImpl::success() const
{
    return success_;
}

} // namespace memo::remote
