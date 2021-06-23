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

std::vector<proto::Memo> MemoDaoImpl::fetchAll()
{
    auto memoCall = factory_->createMemoSearchCall();
    success_ = false;
    if (!memoCall)
        return {};

    success_ = memoCall->exec();
    std::vector<proto::Memo> result;
    if (success_)
    {
        const auto& memos = memoCall->getReply().memos();
        result.assign(memos.begin(), memos.end());
    }
    return result;
}

proto::MemoCreateRs MemoDaoImpl::add(const proto::Memo& memo)
{
    auto memoCall = factory_->createMemoCreateCall();
    success_ = false;
    proto::MemoCreateRs result;
    result.mutable_operation_status()->set_type(proto::OperationStatus::FAILURE);
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
