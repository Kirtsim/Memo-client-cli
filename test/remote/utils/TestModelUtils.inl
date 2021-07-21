#pragma once

#include "remote/utils/ModelUtils.hpp"
#include "model/Tag.hpp"
#include "utils/ModelMapper.hpp"
#include "MemoSvc.pb.h"
#include "TagSvc.pb.h"
#include <gtest/gtest.h>

#include <iostream>
#include <map>
#include <vector>


using namespace memo::remote;
using namespace memo;

using TagMap = std::map<unsigned long, model::TagPtr>;
using TagVector = std::vector<model::TagPtr>;
using MemoVector = std::vector<model::MemoPtr>;

namespace {
    model::TagPtr ToTagPtr(const proto::Tag& tag);

    model::MemoPtr ToMemoPtr(const proto::Memo& memo, const TagMap& mappedTags);

    std::string PrettyPrint(const model::Tag& tag);

    std::string PrettyPrint(const model::Memo& memo);

    proto::Tag CreateProtoTag(unsigned long id);

    proto::Memo CreateProtoMemo(unsigned long id, const std::vector<unsigned long>& tagIds);

    bool AreEqual(const model::TagPtr& actual, const model::TagPtr& expected, std::string& message);

    bool AreEqual(const model::MemoPtr& actual, const model::MemoPtr& expected, std::string& message);

    bool AreEqual(const TagMap& actual, const TagMap& expected, std::string& message);

    bool AreEqual(const TagVector& actual, const TagVector& expected, std::string& message);

    bool AreEqual(const MemoVector& actual, const MemoVector& expected, std::string& message);

} // namespace

TEST(TestModelUtils, extractMappedTags_Response_with_2_tags_Return_2_tags)
{
    const auto tagA = CreateProtoTag(1);
    const auto tagB = CreateProtoTag(2);

    proto::ListMemosRs grpcResponse;
    grpcResponse.mutable_tags()->insert({ tagA.id(), tagA });
    grpcResponse.mutable_tags()->insert({ tagB.id(), tagB });

    auto mappedTags = remote::utils::ExtractMappedTags(grpcResponse);

    auto expectedTagA = ToTagPtr(tagA);
    auto expectedTagB = ToTagPtr(tagB);

    std::string failMsg;
    TagMap expectedTags { { tagA.id(), expectedTagA }, { tagB.id(), expectedTagB } };
    EXPECT_TRUE(AreEqual(mappedTags, expectedTags, failMsg)) << failMsg;
}

TEST(TestModelUtils, extractMappedTags_Response_with_no_tags_Return_empty_map)
{
    proto::ListMemosRs grpcResponse;
    const auto mappedTags = remote::utils::ExtractMappedTags(grpcResponse);
    EXPECT_EQ(mappedTags.size(), 0);
}

TEST(TestModelUtils, ExtractMemos_All_response_tags_found_in_tag_map)
{
    const auto protoTagA = CreateProtoTag(1);
    const auto protoTagB = CreateProtoTag(2);
    const auto protoMemoA = CreateProtoMemo(1, { protoTagA.id(), protoTagB.id() });
    const auto protoMemoB = CreateProtoMemo(2, { protoTagB.id() });

    proto::ListMemosRs grpcResponse;
    grpcResponse.mutable_tags()->insert({ protoTagA.id(), protoTagA });
    grpcResponse.mutable_tags()->insert({ protoTagB.id(), protoTagB });
    (*grpcResponse.add_memos()) = protoMemoA;
    (*grpcResponse.add_memos()) = protoMemoB;

    const TagMap mappedTags { {protoTagA.id(), ToTagPtr(protoTagA) }, {protoTagB.id(), ToTagPtr(protoTagB) }};
    const MemoVector expectedMemos = {
            ToMemoPtr(protoMemoA, mappedTags),
            ToMemoPtr(protoMemoB, mappedTags)
    };

    std::string failMsg;
    auto actualMemos = remote::utils::ExtractMemos(grpcResponse, mappedTags);
    EXPECT_TRUE(AreEqual(actualMemos, expectedMemos, failMsg)) << failMsg;
}

TEST(TestModelUtils, ExtractMemos_Response_with_no_memos_Return_empty_list)
{
    const auto protoTagA = CreateProtoTag(1);
    const auto protoTagB = CreateProtoTag(2);
    proto::ListMemosRs grpcResponse;
    grpcResponse.mutable_tags()->insert({ protoTagA.id(), protoTagA });
    grpcResponse.mutable_tags()->insert({ protoTagB.id(), protoTagB });

    const TagMap mappedTags { {protoTagA.id(), ToTagPtr(protoTagA) }, {protoTagB.id(), ToTagPtr(protoTagB) }};

    auto actualMemos = remote::utils::ExtractMemos(grpcResponse, mappedTags);
    EXPECT_EQ(actualMemos.size(), 0u);
}

TEST(TestModelUtils, ExtractMemos_Response_tag_not_found_in_provided_tag_map_Return_memo_without_that_tag)
{
    const auto protoTagA = CreateProtoTag(1);
    const auto protoTagB = CreateProtoTag(2);
    const auto protoMemo = CreateProtoMemo(1, { protoTagA.id(), protoTagB.id() });

    proto::ListMemosRs grpcResponse;
    grpcResponse.mutable_tags()->insert({ protoTagA.id(), protoTagA });
    grpcResponse.mutable_tags()->insert({ protoTagB.id(), protoTagB });
    (*grpcResponse.add_memos()) = protoMemo;

    const TagMap mappedTags {{ protoTagB.id(), ToTagPtr(protoTagB) }};
    MemoVector expectedMemos { ToMemoPtr(protoMemo, mappedTags) };
    expectedMemos.front()->setTags({ ToTagPtr(protoTagB) });

    std::string message;
    auto actualMemos = remote::utils::ExtractMemos(grpcResponse, mappedTags);
    EXPECT_TRUE(AreEqual(actualMemos, expectedMemos, message)) << message;
}

TEST(TestModelUtils, ExtractMemos_Tag_map_contains_nullptr_Return_memos_without_that_tag)
{
    const auto protoTagA = CreateProtoTag(1);
    const auto protoTagB = CreateProtoTag(2);
    const auto protoMemo = CreateProtoMemo(1, { protoTagA.id(), protoTagB.id() });

    proto::ListMemosRs grpcResponse;
    grpcResponse.mutable_tags()->insert({ protoTagA.id(), protoTagA });
    grpcResponse.mutable_tags()->insert({ protoTagB.id(), protoTagB });
    (*grpcResponse.add_memos()) = protoMemo;

    const TagMap mappedTags { { protoTagA.id(), nullptr }, { protoTagB.id(), ToTagPtr(protoTagB) }};
    const TagMap expectedTags { { protoTagB.id(), ToTagPtr(protoTagB) } };

    MemoVector expectedMemos { ToMemoPtr(protoMemo, expectedTags) };
    expectedMemos.front()->setTags({ ToTagPtr(protoTagB) });

    std::string message;
    const auto actualMemos = remote::utils::ExtractMemos(grpcResponse, mappedTags);
    EXPECT_TRUE(AreEqual(actualMemos, expectedMemos, message)) << message;
}

TEST(TestModelUtils, ExtractMemos_Base_scenario)
{
    const auto protoTagA = CreateProtoTag(1);
    const auto protoTagB = CreateProtoTag(2);
    const auto protoMemoA = CreateProtoMemo(1, { protoTagB.id() });
    const auto protoMemoB = CreateProtoMemo(2, { protoTagA.id(), protoTagB.id() });

    proto::ListMemosRs grpcResponse;
    grpcResponse.mutable_tags()->insert({ protoTagA.id(), protoTagA });
    grpcResponse.mutable_tags()->insert({ protoTagB.id(), protoTagB });
    (*grpcResponse.add_memos()) = protoMemoA;
    (*grpcResponse.add_memos()) = protoMemoB;

    const TagMap mappedTags { { protoTagA.id(), ToTagPtr(protoTagA) }, { protoTagB.id(), ToTagPtr(protoTagB) }};
    const MemoVector expectedMemos = { ToMemoPtr(protoMemoA, mappedTags), ToMemoPtr(protoMemoB, mappedTags) };
    std::string message;
    const auto actualMemos = remote::utils::ExtractMemos(grpcResponse);
    EXPECT_TRUE(AreEqual(actualMemos, expectedMemos, message)) << message;
}

TEST(TestModelUtils, ExtractMemos_Response_is_missing_tag_Skip_that_tag)
{
    const auto protoTagA = CreateProtoTag(1);
    const auto protoMemoA = CreateProtoMemo(1, { protoTagA.id() });
    const auto protoMemoB = CreateProtoMemo(2, { protoTagA.id(), 2 });

    proto::ListMemosRs grpcResponse;
    grpcResponse.mutable_tags()->insert({ protoTagA.id(), protoTagA });
    (*grpcResponse.add_memos()) = protoMemoA;
    (*grpcResponse.add_memos()) = protoMemoB;

    const TagMap mappedTags { { protoTagA.id(), ToTagPtr(protoTagA) } };
    const MemoVector expectedMemos = { ToMemoPtr(protoMemoA, mappedTags), ToMemoPtr(protoMemoB, mappedTags) };
    std::string message;
    const auto actualMemos = remote::utils::ExtractMemos(grpcResponse);
    EXPECT_TRUE(AreEqual(actualMemos, expectedMemos, message)) << message;
}

TEST(TestModelUtils, ExtractTags_Return_proto_tags_converted_to_model_tags)
{
    const auto protoTagA = CreateProtoTag(1);
    const auto protoTagB = CreateProtoTag(2);
    const auto protoTagC = CreateProtoTag(3);
    proto::ListTagsRs grpcResponse;
    (*grpcResponse.add_tags()) = protoTagA;
    (*grpcResponse.add_tags()) = protoTagB;
    (*grpcResponse.add_tags()) = protoTagC;

    const std::vector<model::TagPtr> expectedTags { ToTagPtr(protoTagA), ToTagPtr(protoTagB), ToTagPtr(protoTagC), };
    const auto actualTags = remote::utils::ExtractTags(grpcResponse);

    std::string message;
    EXPECT_TRUE(AreEqual(actualTags, expectedTags, message)) << message;
}

TEST(TestModelUtils, ExtractTags_No_tags_in_response_Return_empty)
{
    proto::ListTagsRs grpcResponse;
    const auto& actualTags = remote::utils::ExtractTags(grpcResponse);
    EXPECT_EQ(actualTags.size(), 0ul);
}

namespace {

model::TagPtr ToTagPtr(const proto::Tag& tag)
{
    return std::make_shared<model::Tag>(Tag::ToModel(tag));
}

model::MemoPtr ToMemoPtr(const proto::Memo& memo, const TagMap& mappedTags)
{
    return std::make_shared<model::Memo>(Memo::ToModel(memo, mappedTags));
}

proto::Tag CreateProtoTag(unsigned long id)
{
    proto::Tag tag;
    tag.set_id(id);
    tag.set_name("Tag" + std::to_string(id));
    tag.set_timestamp((id % 1000) * 1000);

    int colourValue = (static_cast<int>(id) * 100) % 255;
    tag.mutable_colour()->set_blue(colourValue);
    tag.mutable_colour()->set_green(colourValue);
    tag.mutable_colour()->set_red(colourValue);
    return tag;
}

proto::Memo CreateProtoMemo(unsigned long id, const std::vector<unsigned long>& tagIds)
{
    proto::Memo memo;
    memo.set_id(id);
    memo.set_title("Memo" + std::to_string(id));
    memo.set_description("Description" + std::to_string(id));
    memo.set_timestamp((id % 1000) * 1000);
    for (const auto tagId : tagIds)
        memo.add_tag_ids(tagId);

    return memo;
}

bool AreEqual(const TagMap& actual, const TagMap& expected, std::string& message)
{
    if (actual.size() != expected.size())
    {
        message = std::string("Map sizes differ.")
                + "\n  Actual: " + std::to_string(actual.size())
                + "\n  Expected: " + std::to_string(expected.size());
        return false;
    }
    for (const auto& [id, tagPtr] : expected)
    {
        auto it = actual.find(id);
        if (it == std::end(actual))
        {
            message = (tagPtr ? PrettyPrint(*tagPtr) : "nullptr") + " not found.";
            return false;
        }
        if (!AreEqual(it->second, tagPtr, message))
            return false;
    }
    return true;
}

bool AreEqual(const TagVector& actual, const TagVector& expected, std::string& message)
{
    if (actual.size() != expected.size())
    {
        message = std::string("Tag counts differ.")
                  + "\n  Actual: " + std::to_string(actual.size())
                  + "\n  Expected: " + std::to_string(expected.size());
        return false;
    }

    for (size_t i = 0; i < expected.size(); ++i)
    {
        if (!AreEqual(actual[i], expected[i], message))
            return false;
    }
    return true;
}

bool AreEqual(const MemoVector& actual, const MemoVector& expected, std::string& message)
{
    if (actual.size() != expected.size())
    {
        message = std::string("Memo counts differ.")
                  + "\n  Actual: " + std::to_string(actual.size())
                  + "\n  Expected: " + std::to_string(expected.size());
        return false;
    }
    for (size_t i = 0; i < actual.size(); ++i)
    {
        if (!AreEqual(actual[i], expected[i], message))
            return false;
    }
    return true;
}

bool AreEqual(const model::TagPtr& actual, const model::TagPtr& expected, std::string& message)
{
    bool areDifferent = ((!actual || !expected) && actual != expected)
            || (*actual != *expected);

    if (areDifferent)
    {
        message = std::string("Tags differ:")
                  + "\n Actual:   " + (actual ? PrettyPrint(*actual) : "nullptr")
                  + "\n Expected: " + (expected ? PrettyPrint(*expected) : "nullptr");
        return false;
    }
    return true;
}

bool AreEqual(const model::MemoPtr& actual, const model::MemoPtr& expected, std::string& message)
{
    bool areDifferent = ((!actual || !expected) && actual != expected)
                        || (*actual != *expected);

    if (areDifferent)
    {
        message = std::string("Memos differ:")
                  + "\n Actual:   " + (actual ? PrettyPrint(*actual) : "nullptr")
                  + "\n Expected: " + (expected ? PrettyPrint(*expected) : "nullptr");
        return false;
    }
    return true;
}

std::string PrettyPrint(const model::Memo& memo)
{
    std::stringstream stream;
    stream << "Memo {" << memo.id()
           << " \"" << memo.title() << "\""
           << " \"" << memo.description() << "\""
           << " " << memo.timestamp() << " (" << memo.tags().size() << ")[";
    int tagNumber = 1;
    for (const auto& tag : memo.tags())
    {
        stream << " " << tagNumber++ << "_";
        if (!tag)
            stream << "nullptr";
        else
            stream << PrettyPrint(*tag) << " ";
    }
    stream << "]}";
    return stream.str();
}

std::string PrettyPrint(const model::Tag& tag)
{
    std::stringstream stream;
    stream << "Tag {" << tag.id()
           << " \"" << tag.name() << "\""
           << " " << tag.timestamp()
           << " {" << tag.colour().red << "," << tag.colour().green << "," << tag.colour().blue << "}"
           << "}";
    return stream.str();
}

} // namespace