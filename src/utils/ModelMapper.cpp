#include "utils/ModelMapper.hpp"

namespace memo {
namespace Memo {

model::Memo ToModel(const proto::Memo& proto, std::map<unsigned long, model::TagPtr>& tags)
{
    model::Memo memo;
    memo.setId(proto.id());
    memo.setTitle(proto.title());
    memo.setDescription(proto.description());
    for (const auto& tagId : proto.tag_ids())
    {
        auto iter = tags.find(tagId);
        if (iter != std::end(tags))
            memo.addTag(iter->second);
    }
    memo.setTimestamp(proto.timestamp());
    return memo;
}

proto::Memo ToProto(const model::Memo& model)
{
    proto::Memo memo;
    memo.set_id(model.id());
    memo.set_title(model.title());
    memo.set_description(model.description());
    for (const auto& tag : model.tags())
    {
        if (tag)
        {
            memo.add_tag_ids(tag->id());
        }
    }
    memo.set_timestamp(model.timestamp());
    return memo;
}

} // namespace Memo

namespace Tag {

model::Tag ToModel(const proto::Tag& proto)
{
    model::Tag tag;
    tag.setName(proto.name());
    tag.setColor(proto.color());
    tag.setTimestamp(proto.timestamp());
    return tag;
}

proto::Tag ToProto(const model::Tag& proto)
{
    proto::Tag tag;
    tag.set_name(proto.name());
    tag.set_color(proto.color());
    tag.set_timestamp(proto.timestamp());
    return tag;
}

} // namespace Tag

} // namespace memo
