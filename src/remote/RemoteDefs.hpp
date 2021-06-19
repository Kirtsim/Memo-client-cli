#pragma once
#include <memory>

namespace memo::remote {

template<class ResponseData>
class IServiceResponse;

template <class ResponseType>
class ServiceResponseImpl;


class ListMemoResponseData;
using ListMemoResponse = IServiceResponse<ListMemoResponseData>;
using ListMemoResponseImpl = ServiceResponseImpl<ListMemoResponseData>;
using ListMemoResponsePtr = std::shared_ptr<ListMemoResponse>;

class AddMemoResponseData;
using AddMemoResponse = IServiceResponse<AddMemoResponseData>;
using AddMemoResponseImpl = ServiceResponseImpl<AddMemoResponseData>;
using AddMemoResponsePtr = std::shared_ptr<AddMemoResponse>;

} // namespace memo::remote