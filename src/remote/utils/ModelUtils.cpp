#include "remote/utils/ModelUtils.hpp"
#include "MemoSvc.pb.h"
#include "utils/ModelMapper.hpp"

namespace memo::remote::utils {

std::vector<model::MemoPtr> ExtractMemos(const proto::ListMemosRs& memoResponse)
{
    const auto tagMap = ExtractMappedTags(memoResponse);
    return ExtractMemos(memoResponse, tagMap);
}

std::vector<model::MemoPtr> ExtractMemos(const proto::ListMemosRs& memoResponse,
                                         const std::map<unsigned long, model::TagPtr>& mappedTags)
{
    std::vector<model::MemoPtr> memos;

    for (const auto& protoMemo : memoResponse.memos())
    {
        const auto memo = Memo::ToModel(protoMemo, mappedTags);
        memos.emplace_back(std::make_shared<model::Memo>(memo));
    }
    return memos;
}

std::map<unsigned long, model::TagPtr> ExtractMappedTags(const proto::ListMemosRs& memoResponse)
{
    std::map<unsigned long, model::TagPtr> tags;

    for (const auto& [id, protoTag] : memoResponse.tags())
    {
        const auto tag = Tag::ToModel(protoTag);
        tags.insert({ id, std::make_shared<model::Tag>(tag) });
    }
    return tags;
}

} // namespace memo::remote