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
    std::string requestUuid_;
    model::MemoPtr memo_;
};

class AddMemoResponseDataBuilder
{
public:
    ~AddMemoResponseDataBuilder() = default;

    AddMemoResponseDataBuilder& setRequestUuid(const std::string& requestUuid);

    AddMemoResponseDataBuilder& setMemo(const model::MemoPtr& memo);

    AddMemoResponseData build() const;

private:
    AddMemoResponseData data_;
};


} // namespace memo::remote


