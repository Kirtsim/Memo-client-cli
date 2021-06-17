#pragma once
#include "remote/RemoteDefs.hpp"

namespace memo::remote {

class ListMemoRequest;
class AddMemoRequest;

class IServiceResponse;
class ListMemoResponseData;


class IMemoService
{
public:
    virtual ~IMemoService() = default;

    virtual ListMemoResponsePtr listMemos(const ListMemoRequest& request) = 0;

    virtual ListMemoResponsePtr addMemo(const AddMemoRequest& request) = 0;

    //virtual ListMemoResponsePtr UpdateMemo(const UpdateMemoRequest& request) = 0;

    //virtual ListMemoResponsePtr RemoveMemo(const RemoveMemoRequest& request) = 0;
};


} // namespace memo::remote