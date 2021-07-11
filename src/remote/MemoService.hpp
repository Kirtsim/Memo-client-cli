#pragma once
#include "remote/RemoteDefs.hpp"

namespace memo::remote {

class ListMemoRequest;
class AddMemoRequest;
class MemoCallFactory;

class IMemoService
{
public:
    virtual ~IMemoService() = default;

    virtual ListMemoResponsePtr listMemos(const ListMemoRequest& request) = 0;

    virtual AddMemoResponsePtr addMemo(const AddMemoRequest& request) = 0;

    //virtual ListMemoResponsePtr UpdateMemo(const UpdateMemoRequest& request) = 0;

    //virtual ListMemoResponsePtr RemoveMemo(const RemoveMemoRequest& request) = 0;
};

class MemoServiceImpl : public IMemoService
{
public:
    static std::shared_ptr<MemoServiceImpl> Create(std::unique_ptr<MemoCallFactory> callFactory);

    explicit MemoServiceImpl(std::unique_ptr<MemoCallFactory> callFactory);

    ListMemoResponsePtr listMemos(const ListMemoRequest& request) override;

    AddMemoResponsePtr addMemo(const AddMemoRequest& request) override;

private:
    std::unique_ptr<MemoCallFactory> callFactory_;
};


} // namespace memo::remote