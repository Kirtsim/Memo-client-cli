#pragma once
#include "remote/RemoteDefs.hpp"

namespace memo::remote {

class ListTagsRequest;
class AddTagRequest;
class ITagCallFactory;

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
    static std::shared_ptr<TagServiceImpl> Create(std::unique_ptr<ITagCallFactory> callFactory);

    explicit TagServiceImpl(std::unique_ptr<ITagCallFactory> callFactory);

    ~TagServiceImpl() override;

    ListTagsResponsePtr listTags(const ListTagsRequest& request) override;

    AddTagResponsePtr addTag(const AddTagRequest& request) override;

private:
    std::unique_ptr<ITagCallFactory> callFactory_;
};
} // namespace memo::remote
