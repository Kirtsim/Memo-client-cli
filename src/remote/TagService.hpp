#pragma once
#include "remote/RemoteDefs.hpp"

namespace memo::remote {

class ListTagsRequest;
class AddTagRequest;

class ITagService
{
public:
    virtual ~ITagService() = default;

    virtual ListTagsResponsePtr listTags(const ListTagsRequest& request) = 0;

    virtual AddTagResponsePtr addTag(const AddTagRequest& request) = 0;

    //virtual ListTagResponsePtr UpdateTag(const UpdateMemoRequest& request) = 0;

    //virtual ListTagResponsePtr RemoveTag(const RemoveMemoRequest& request) = 0;
};

class TagServiceImpl : public ITagService
{
public:
    ListTagsResponsePtr listTags(const ListTagsRequest& request) override;

    AddTagResponsePtr addTag(const AddTagRequest& request) override;
};
} // namespace memo::remote
