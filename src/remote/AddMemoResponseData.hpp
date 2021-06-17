#pragma once
#include "model/ModelDefs.hpp"

#include <string>


namespace memo::remote {

class AddMemoResponseData
{
    friend class AddMemoResponseDataBuilder;

public:
    const std::string& requestUuid() const;

    const model::MemoPtr& memo() const;

private:
    void setRequestUuid(const std::string& requestUuid);

    void setMemo(const model::MemoPtr& memo);

private:
    std::string requestUuid_;
    model::MemoPtr memo_;
};

} // namespace memo::remote


