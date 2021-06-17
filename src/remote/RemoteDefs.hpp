#pragma once
#include <memory>

namespace memo::remote {

class IServiceResponse;
class ListMemoteResponseData;

using ListMemoResponsePtr = std::shared_ptr<ListMemoteResponseData>;

} // namespace memo::remote