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


class ListTagsResponseData;
using ListTagsResponse = IServiceResponse<ListTagsResponseData>;
using ListTagsResponseImpl = ServiceResponseImpl<ListTagsResponseData>;
using ListTagsResponsePtr = std::shared_ptr<ListTagsResponse>;

class AddTagResponseData;
using AddTagResponse = IServiceResponse<AddTagResponseData>;
using AddTagResponseImpl = ServiceResponseImpl<AddTagResponseData>;
using AddTagResponsePtr = std::shared_ptr<AddTagResponse>;
} // namespace memo::remote