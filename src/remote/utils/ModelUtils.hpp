#pragma once
#include "model/ModelDefs.hpp"

#include <map>
#include <vector>
namespace memo::proto {
    class ListMemosRs;
}
namespace memo::remote::utils {

/// @brief Collects memos from a ListMemoRs and converts them into "model" memos.
/// If the response does not contain "proto" tags, the resulting memos will contain
/// no tags.
/// @param memoResponse response from the ListMemos service call.
/// @return a vector of model memos found in the response.
std::vector<model::MemoPtr> ExtractMemos(const proto::ListMemosRs& memoResponse);

/// @brief Collects memos from a ListMemoRs and converts them into "model" memos.
/// This function does not create nor attempts to create "model" tags even if "proto"
/// tags are present in the response.
/// Tags in the provided map are used to populate the tag lists memos' should they
/// should they contain any. If a tag is not found in the map, that tag is not added
/// to the memo.
/// @param memoResponse response from the ListMemos service call.
/// @return a vector of model memos found in the response.
std::vector<model::MemoPtr> ExtractMemos(const proto::ListMemosRs& memoResponse,
                                         const std::map<unsigned long, model::TagPtr>& mappedTags);

/// @brief Collects tags from a ListMemoRs an converts them into "model" tags.
/// If the response comes without "proto" tags (still valid) the resulting map will
/// be empty.
/// @param memoResponse response from the ListMemos service call.
/// @return a map where the key is the tag's id and the value is a tag pointer.
std::map<unsigned long, model::TagPtr> ExtractMappedTags(const proto::ListMemosRs& memoResponse);


} // namespace memo::remote