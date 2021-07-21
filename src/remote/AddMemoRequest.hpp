#pragma once
#include "model/ModelDefs.hpp"
#include <string>

namespace memo::remote {

class AddMemoRequest
{
    friend class AddMemoRequestBuilder;
public:
    const std::string& uuid() const;

    const model::MemoPtr& memo() const;

private:
    void setUuid(const std::string& uuid);

    void setMemo(const model::MemoPtr& memo);

private:
    std::string uuid_;
    model::MemoPtr memo_;
};

class AddMemoRequestBuilder
{
public:
    AddMemoRequestBuilder& setRequestUuid(const std::string& uuid);

    AddMemoRequestBuilder& setMemo(const model::MemoPtr& memo);

    AddMemoRequest build();

private:
    AddMemoRequest request_;
};

} // namespace memo::remote
