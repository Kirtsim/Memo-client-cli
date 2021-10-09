#include <remote/utils/Mapping.hpp>
#include <remote/utils/ModelUtils.hpp>
#include "remote/TagService.hpp"
#include "remote/factory/TagCallFactory.hpp"
#include "remote/ServiceResponse.hpp"
#include "remote/ListTagsCall.hpp"
#include "remote/AddTagCall.hpp"
#include "remote/ListTagsResponseData.hpp"
#include "remote/AddTagResponseData.hpp"
#include "remote/ListTagsRequest.hpp"
#include "remote/AddTagRequest.hpp"
#include "utils/ModelMapper.hpp"

#include "TagSvc.pb.h"

namespace memo::remote {

std::shared_ptr<TagServiceImpl> TagServiceImpl::Create(std::unique_ptr<ITagCallFactory> callFactory)
{
    return std::make_shared<TagServiceImpl>(std::move(callFactory));
}

TagServiceImpl::TagServiceImpl(std::unique_ptr<ITagCallFactory> callFactory)
    : callFactory_(std::move(callFactory))
{
}

TagServiceImpl::~TagServiceImpl() = default;

ListTagsResponsePtr TagServiceImpl::listTags(const ListTagsRequest& request)
{
    ServiceResponseBuilder<ListTagsResponseData> responseBuilder;
    auto tagCall = callFactory_->createListTagsCall();
    if (!tagCall)
    {
        return responseBuilder.setStatus(ResponseStatus::kError)
                              .setStatusMessage("Failed to contact the server.")
                              .build();
    }

    proto::ListTagsRq grpcRequest;
    grpcRequest.set_uuid(request.uuid());
    grpcRequest.set_view(utils::Model2Proto(request.view()));
    grpcRequest.mutable_filter()->set_name_starts_with(request.filter().nameStartsWith);
    grpcRequest.mutable_filter()->set_contains(request.filter().contains);
    for (const auto& color : request.filter().colours)
        grpcRequest.mutable_filter()->add_colours(Color2Int(color));

    for (const auto memoId : request.filter().assignedToMemos)
        grpcRequest.mutable_filter()->add_assigned_to_memos(memoId);

    grpcRequest.mutable_filter()->mutable_creation_time()->set_start(request.filter().dateFrom);
    grpcRequest.mutable_filter()->mutable_creation_time()->set_end(request.filter().dateUntil);
    if (!request.pageToken().empty())
        grpcRequest.set_page_token(request.pageToken());
    grpcRequest.set_result_page_size(request.resultPageSize());

    tagCall->setRequest(grpcRequest);

    bool success = tagCall->exec();
    if (!success)
    {
        return responseBuilder.setStatus(ResponseStatus::kError)
                              .setStatusMessage("Failed to contact the server.")
                              .build();
    }
    const auto& grpcResponse = tagCall->getReply();
    responseBuilder.setRequestId(grpcResponse.request_uuid())
                   .setCode(0)
                   .setStatus(ResponseStatus::kSuccess);

    const auto tags = utils::ExtractTags(grpcResponse);
    ListTagsResponseDataBuilder dataBuilder;
    dataBuilder.setRequestUuid(grpcResponse.request_uuid())
               .setTags(tags)
               .setTotalCount(grpcResponse.total_count())
               .setNextPageToken(grpcResponse.next_page_token())
               .setPrevPageToken(grpcResponse.prev_page_token());
    const auto data = dataBuilder.build();
    responseBuilder.setData(data);
    return responseBuilder.build();
}

AddTagResponsePtr TagServiceImpl::addTag(const AddTagRequest& request)
{
    ServiceResponseBuilder<AddTagResponseData> responseBuilder;
    auto call = callFactory_->createAddTagCall();
    if (!call)
    {
        return responseBuilder.setStatus(ResponseStatus::kError)
                              .setStatusMessage("Failed to contact the server.")
                              .build();
    }
    proto::AddTagRq grpcRequest;
    grpcRequest.set_uuid(request.uuid());
    if (const auto& modelTag = request.tag())
        (*grpcRequest.mutable_tag()) = Tag::ToProto(*modelTag);

    call->setRequest(grpcRequest);
    bool success = call->exec();
    if (!success)
    {
        return responseBuilder.setStatus(ResponseStatus::kError)
                              .setStatusMessage("Failed to contact the server.")
                              .build();
    }

    const auto& grpcResponse = call->getReply();
    responseBuilder.setRequestId(grpcResponse.request_uuid())
                   .setStatus(utils::Proto2Model(grpcResponse.operation_status().type()))
                   .setCode(grpcResponse.operation_status().code())
                   .setStatusMessage(grpcResponse.operation_status().comment());
    AddTagResponseDataBuilder dataBuilder;
    dataBuilder.setRequestUuid(grpcResponse.request_uuid())
               .setTag(std::make_shared<model::Tag>(Tag::ToModel(grpcResponse.tag())));
    responseBuilder.setData(dataBuilder.build());

    return responseBuilder.build();
}

} // namespace memo::remote
