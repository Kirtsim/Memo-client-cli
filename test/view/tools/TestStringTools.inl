#pragma once
#include "view/tools/StringTools.hpp"
#include <gtest/gtest.h>

using namespace memo;
using namespace memo::tools;

TEST(TestStringTools, splitIntoLines_From_empty_string_return_a_vector_with_an_empty_string)
{
    auto lines = splitIntoLines("", 100);
    ASSERT_FALSE(lines.empty());
    EXPECT_EQ(lines.size(), 1);
    EXPECT_TRUE(lines.front().empty());
}

TEST(TestStringTools, splitIntoLines_From_simple_text_return_one_line_with_the_text)
{
    std::string inputText = "This is my text for testing.";
    auto lines = splitIntoLines(inputText, 100);
    ASSERT_FALSE(lines.empty());
    EXPECT_EQ(lines.size(), 1);
    EXPECT_EQ(lines.front(), inputText);
}

TEST(TestStringTools, splitIntoLines_From_text_containing_only_a_lineFeed_return_two_empty_lines)
{
    auto lines = splitIntoLines("\n", 100);
    ASSERT_FALSE(lines.empty());
    EXPECT_EQ(lines.size(), 2);
    EXPECT_EQ(lines.front(), "");
    EXPECT_EQ(lines.back(), "");
}

TEST(TestStringTools, splitIntoLines_From_text_containing_only_lineFeeds_return_empty_lines)
{
    auto lines = splitIntoLines("\n\n\n\n", 100);
    ASSERT_FALSE(lines.empty());
    EXPECT_EQ(lines.size(), 5);
    for (size_t i = 0; i < lines.size(); ++i)
    {
        EXPECT_EQ(lines[i], "") << "On index " << i << ".";
    }
}

TEST(TestStringTools, splitIntoLines_From_text_with_lineFeed_at_the_end_return_two_lines)
{
    const std::string expectedFirstLine = "This is my text for testing.";
    const auto inputText = expectedFirstLine + "\n";

    auto lines = splitIntoLines(inputText, 100);
    ASSERT_FALSE(lines.empty());
    EXPECT_EQ(lines.size(), 2);
    EXPECT_EQ(lines.front(), expectedFirstLine);
    EXPECT_EQ(lines.back(), "");
}

TEST(TestStringTools, splitIntoLines_From_text_with_lineFeeds_in_middle_return_lines_with_empty_text_in_middle)
{
    const std::string expectedFirstLine = "This is the first line.";
    const std::string expectedLastLine = "This is the last line.";
    const auto inputText = expectedFirstLine + "\n\n\n" + expectedLastLine;

    auto lines = splitIntoLines(inputText, 100);
    ASSERT_FALSE(lines.empty());
    EXPECT_EQ(lines.size(), 4);
    EXPECT_EQ(lines.front(), expectedFirstLine);

    ASSERT_TRUE(lines.size() > 1);
    EXPECT_EQ(lines[1], "");
    ASSERT_TRUE(lines.size() > 2);
    EXPECT_EQ(lines[2], "");
    ASSERT_TRUE(lines.size() > 3);
    EXPECT_EQ(lines[3], expectedLastLine);
}

TEST(TestStringTools, splitIntoLines_Line_is_longer_than_max_width_Return_the_text_split_in_two_lines)
{
    const std::string expectedFirstLine = "12345";
    const std::string expectedSecondLine = "678";

    const auto inputText = expectedFirstLine + expectedSecondLine;

    auto lines = splitIntoLines(inputText, 5);
    ASSERT_FALSE(lines.empty());
    EXPECT_EQ(lines.size(), 2);
    EXPECT_EQ(lines.front(), expectedFirstLine);
    EXPECT_EQ(lines.back(), expectedSecondLine);
}

TEST(TestStringTools, splitIntoLines_Line_length_equals_max_width_Return_the_text_on_first_line_plus_empty_line)
{
    const std::string inputText = "12345";

    auto lines = splitIntoLines(inputText, 5);
    ASSERT_FALSE(lines.empty());
    EXPECT_EQ(lines.size(), 2);
    EXPECT_EQ(lines.front(), inputText);
    EXPECT_EQ(lines.back(), "");
}

TEST(TestStringTools, splitIntoLines_Line_length_equals_max_width_with_LineFeed_at_the_end_Return_the_text_on_first_line_plus_two_empty_lines)
{
    const std::string firstExpectedLine = "12345";
    const std::string inputText = firstExpectedLine + "\n";

    auto lines = splitIntoLines(inputText, 5);
    ASSERT_FALSE(lines.empty());
    EXPECT_EQ(lines.size(), 3);
    EXPECT_EQ(lines.front(), firstExpectedLine);
    for (size_t i = 1; i < lines.size(); ++i)
        EXPECT_EQ(lines[i], "");
}

// TODO: failing test
TEST(TestStringTools, DISABLED_splitIntoLines_For_non_empty_text_and_0_available_width_treat_does_not_impose_text_wrap)
{
    const std::string firstExpectedLine = "Some extra long text that would have otherwise had to be wrapped on a new line.";
    const std::string secondExpectedLine = "This text is deliberately placed on a new line.";
    const auto inputText = firstExpectedLine + "\n" + secondExpectedLine +"\n";
    auto lines = splitIntoLines(inputText, 0);

    ASSERT_FALSE(lines.empty());
    EXPECT_EQ(lines.size(), 3);
    EXPECT_EQ(lines.front(), firstExpectedLine);
    ASSERT_TRUE(lines.size() > 1);
    EXPECT_EQ(lines[1], secondExpectedLine);
    ASSERT_TRUE(lines.size() > 2);
    EXPECT_EQ(lines[2], "");
}
