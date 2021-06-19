#include "remote/MemoService.hpp"
#include "remote/AddMemoRequest.hpp"
#include "remote/ListMemoRequest.hpp"
#include "remote/ServiceResponse.hpp"
#include "remote/ListMemoResponseData.hpp"
#include "remote/AddMemoResponseData.hpp"

namespace memo::remote {

ListMemoResponsePtr MemoServiceImpl::listMemos(const ListMemoRequest& request)
{
    ServiceResponseBuilder<ListMemoResponseData> responseBuilder;

    return responseBuilder.build();
}

AddMemoResponsePtr MemoServiceImpl::addMemo(const AddMemoRequest& request)
{
    ServiceResponseBuilder<AddMemoResponseData> responseBuilder;

    return responseBuilder.build();
}
} // namespace memo::remote
