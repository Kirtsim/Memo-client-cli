#pragma once
#include "model/ModelDefs.hpp"

#include <map>
#include <vector>
namespace memo::proto {
    class ListMemosRs;
}
namespace memo::remote::utils {

std::vector<model::MemoPtr> ExtractMemos(const proto::ListMemosRs& memoResponse);

std::vector<model::MemoPtr> ExtractMemos(const proto::ListMemosRs& memoResponse,
                                         const std::map<unsigned long, model::TagPtr>& mappedTags);

std::map<unsigned long, model::TagPtr> ExtractMappedTags(const proto::ListMemosRs& memoResponse);


} // namespace memo::remote