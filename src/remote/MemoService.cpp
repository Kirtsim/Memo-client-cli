#include "remote/MemoService.hpp"
#include "remote/AddMemoRequest.hpp"
#include "remote/ListMemoRequest.hpp"
#include "remote/ServiceResponse.hpp"
#include "remote/ListMemoResponseData.hpp"
#include "remote/AddMemoResponseData.hpp"
#include "remote/factory/MemoCallFactory.hpp"
#include "remote/ListMemosCall.hpp"
#include "remote/AddMemoCall.hpp"
#include "remote/ServiceEnums.hpp"
#include "remote/utils/Mapping.hpp"
#include "remote/utils/ModelUtils.hpp"
#include "utils/ModelMapper.hpp"

#include "MemoSvc.pb.h"

namespace memo::remote {

std::shared_ptr<MemoServiceImpl> MemoServiceImpl::Create(std::unique_ptr<MemoCallFactory> callFactory)
{
    return std::make_shared<MemoServiceImpl>(std::move(callFactory));
}

MemoServiceImpl::MemoServiceImpl(std::unique_ptr<MemoCallFactory> callFactory)
    : callFactory_(std::move(callFactory))
{
}

ListMemoResponsePtr MemoServiceImpl::listMemos(const ListMemoRequest& request)
{
    ServiceResponseBuilder<ListMemoResponseData> responseBuilder;

    auto memoCall = callFactory_->createListMemosCall();
    if (!memoCall)
    {
        return responseBuilder.setStatus(ResponseStatus::kError)
                              .setStatusMessage("Failed to contact the server.")
                              .build();
    }

    proto::ListMemosRq grpcRequest;
    grpcRequest.set_uuid(request.uuid());
    grpcRequest.set_view(utils::Model2Proto(request.memoView()));
    grpcRequest.mutable_filter()->set_title_starts_with(request.filter().titleStartsWith);
    for (const auto& keyWord : request.filter().titleContains)
        grpcRequest.mutable_filter()->add_title_contains(keyWord);
    for (const auto& keyWord : request.filter().contains)
        grpcRequest.mutable_filter()->add_contains(keyWord);
    for (const auto tagId : request.filter().taggedBy)
        grpcRequest.mutable_filter()->add_tagged_by(tagId);
    grpcRequest.mutable_filter()->mutable_creation_time()->set_start(request.filter().dateFrom);
    grpcRequest.mutable_filter()->mutable_creation_time()->set_end(request.filter().dateUntil);
    if (!request.pageToken().empty())
        grpcRequest.set_page_token(request.pageToken());
    grpcRequest.set_result_page_size(request.pageSize());
    memoCall->setRequest(grpcRequest);

    bool success = memoCall->exec();
    if (!success)
    {
        return responseBuilder.setStatus(ResponseStatus::kError)
                              .setStatusMessage("Failed to contact the server.")
                              .build();
    }
    const auto& grpcResponse = memoCall->getReply();
    responseBuilder.setRequestId(grpcResponse.request_uuid())
                   .setCode(0)
                   .setStatus(ResponseStatus::kSuccess);

    const auto tags  = utils::ExtractMappedTags(grpcResponse);
    const auto memos = utils::ExtractMemos(grpcResponse, tags);
    ListMemoResponseDataBuilder dataBuilder;
    dataBuilder.setRequestUuid(grpcResponse.request_uuid())
               .setMemos(memos)
               .setTags(tags)
               .setTotalCount(static_cast<size_t>(grpcResponse.total_count()))
               .setNextPageToken(grpcResponse.next_page_token())
               .setPrevPageToken(grpcResponse.prev_page_token());
    const auto data = dataBuilder.build();
    responseBuilder.setData(data);
    return responseBuilder.build();
}

AddMemoResponsePtr MemoServiceImpl::addMemo(const AddMemoRequest& request)
{
    ServiceResponseBuilder<AddMemoResponseData> responseBuilder;
    auto call = callFactory_->createAddMemoCall();
    if (!call)
    {
        return responseBuilder.setStatus(ResponseStatus::kError)
                              .setStatusMessage("Failed to contact the server.")
                              .build();
    }
    proto::AddMemoRq grpcRequest;
    if (const auto& memo = request.memo())
        (*grpcRequest.mutable_memo()) = Memo::ToProto(*memo);
    grpcRequest.set_uuid(request.uuid());

    call->setRequest(grpcRequest);
    bool success = call->exec();
    if (!success)
    {
        responseBuilder.setStatus(ResponseStatus::kError);
        responseBuilder.setStatusMessage("Failed to contact the server.");
        return responseBuilder.build();
    }

    const auto& grpcResponse = call->getReply();
    responseBuilder.setCode(grpcResponse.operation_status().code())
                   .setStatus(utils::Proto2Model(grpcResponse.operation_status().type()))
                   .setStatusMessage(grpcResponse.operation_status().comment())
                   .setRequestId(grpcResponse.request_uuid());

    AddMemoResponseDataBuilder dataBuilder;
    dataBuilder.setRequestUuid(grpcResponse.request_uuid());
    const auto& sentMemoTags = request.memo()->tags();
    const auto& tagIds = grpcResponse.added_memo().tag_ids();
    std::map<unsigned long, model::TagPtr> idTagMap;
    for (const auto& tag : sentMemoTags)
    {
        if (!tag)
            continue;
        auto it = std::find(tagIds.begin(), tagIds.end(), tag->id());
        if (it != std::end(tagIds))
        {
            idTagMap.insert({ tag->id(), tag });
        }
    }

    auto memo = std::make_shared<model::Memo>(Memo::ToModel(grpcResponse.added_memo(), idTagMap));
    dataBuilder.setMemo(memo);

    const auto data = dataBuilder.build();
    responseBuilder.setData(data);
    return responseBuilder.build();
}

} // namespace memo::remote
