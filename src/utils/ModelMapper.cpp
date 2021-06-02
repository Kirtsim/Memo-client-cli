#include "ModelMapper.hpp"

namespace memo {
namespace Memo {

model::Memo ToModel(const proto::Memo& proto)
{
    model::Memo memo;
    memo.setId(proto.id());
    memo.setTitle(proto.title());
    memo.setDescription(proto.description());
    for (const auto& tagName : proto.tag_names())
        memo.addTagName(tagName);
    memo.setTimestamp(static_cast<unsigned long>(proto.timestamp()));
    return memo;
}

proto::Memo ToProto(const model::Memo& model)
{
    proto::Memo memo;
    memo.set_id(model.id());
    memo.set_title(model.title());
    memo.set_description(model.description());
    for (const auto& tagName : model.tagNames())
        memo.add_tag_names(tagName);
    memo.set_timestamp(static_cast<long>(model.timestamp()));
    return memo;
}

} // namespace Memo

namespace Tag {

model::Tag ToModel(const proto::Tag& proto)
{
    model::Tag tag;
    tag.setName(proto.name());
    tag.setColor(proto.color());
    tag.setTimestamp(static_cast<unsigned long>(proto.timestamp()));
    return tag;
}

proto::Tag ToProto(const model::Tag& proto)
{
    proto::Tag tag;
    tag.set_name(proto.name());
    tag.set_color(proto.color());
    tag.set_timestamp(static_cast<long>(proto.timestamp()));
    return tag;
}

} // namespace Tag

} // namespace memo
