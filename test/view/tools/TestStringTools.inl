#pragma once
#include "view/tools/StringTools.hpp"
#include <gtest/gtest.h>

using namespace memo;
using namespace memo::tools;
namespace {
    const bool kApplyWordWrap = true;
} // namespace

TEST(TestStringTools, splitIntoLines_From_empty_string_Return_a_vector_with_an_empty_string)
{
    auto lines = splitIntoLines("", 100);
    auto linesWithWordWrap = splitIntoLines("", 100, kApplyWordWrap);
    ASSERT_FALSE(lines.empty());
    EXPECT_EQ(lines.size(), 1);
    EXPECT_TRUE(lines.front().empty());
    EXPECT_EQ(lines, linesWithWordWrap);
}

TEST(TestStringTools, splitIntoLines_From_simple_text_Return_one_line_with_the_text)
{
    std::string inputText = "This is my text for testing.";
    auto lines = splitIntoLines(inputText, 100);
    auto linesWithWordWrap = splitIntoLines(inputText, 100, kApplyWordWrap);
    ASSERT_FALSE(lines.empty());
    EXPECT_EQ(lines.size(), 1);
    EXPECT_EQ(lines.front(), inputText);
    EXPECT_EQ(lines, linesWithWordWrap);
}

TEST(TestStringTools, splitIntoLines_From_text_containing_only_a_lineFeed_return_two_empty_lines)
{
    const std::vector<std::string> expectedLines { "", "" };
    auto lines = splitIntoLines("\n", 100);
    auto linesWithWordWrap = splitIntoLines("\n", 100, kApplyWordWrap);

    EXPECT_EQ(lines.size(), expectedLines.size());
    EXPECT_EQ(lines, expectedLines);

    EXPECT_EQ(linesWithWordWrap.size(), expectedLines.size());
    EXPECT_EQ(linesWithWordWrap, expectedLines);
}

TEST(TestStringTools, splitIntoLines_From_text_containing_only_lineFeeds_return_empty_lines)
{
    const std::string inputText = "\n\n\n\n";
    const std::vector<std::string> expectedLines { "", "", "", "", "" };
    auto lines = splitIntoLines(inputText, 100);
    auto linesWithWordWrap = splitIntoLines(inputText, 100, kApplyWordWrap);

    EXPECT_EQ(lines.size(), expectedLines.size());
    EXPECT_EQ(lines, expectedLines);

    EXPECT_EQ(linesWithWordWrap.size(), expectedLines.size());
    EXPECT_EQ(linesWithWordWrap, expectedLines);
}

TEST(TestStringTools, splitIntoLines_From_text_with_lineFeed_at_the_end_return_two_lines)
{
    const std::vector<std::string> expectedLines { "This is my text for testing.", "" };
    const auto inputText = expectedLines[0] + "\n";

    auto lines = splitIntoLines(inputText, 100);
    auto linesWithWordWrap = splitIntoLines(inputText, 100, kApplyWordWrap);

    EXPECT_EQ(lines.size(), expectedLines.size());
    EXPECT_EQ(lines, expectedLines);

    EXPECT_EQ(linesWithWordWrap.size(), expectedLines.size());
    EXPECT_EQ(linesWithWordWrap, expectedLines);
}

TEST(TestStringTools, splitIntoLines_From_text_with_lineFeeds_in_middle_return_lines_with_empty_text_in_middle)
{
    const std::vector<std::string> expectedLines { "This is the first line.", "", "", "This is the last line." };
    const auto inputText = expectedLines.front() + "\n\n\n" + expectedLines.back();

    auto lines = splitIntoLines(inputText, 100);
    auto linesWithWordWrap = splitIntoLines(inputText, 100, kApplyWordWrap);
    EXPECT_EQ(lines.size(), expectedLines.size());
    EXPECT_EQ(lines, expectedLines);

    EXPECT_EQ(linesWithWordWrap.size(), expectedLines.size());
    EXPECT_EQ(linesWithWordWrap, expectedLines);
}

TEST(TestStringTools, splitIntoLines_Apply_wordWrap_on_long_line_Return_the_text_split_in_two_lines)
{
    const std::vector<std::string> expectedLines { "12345", "678" };
    const auto inputText = expectedLines.front() + expectedLines.back();

    auto lines = splitIntoLines(inputText, 5, kApplyWordWrap);
    EXPECT_EQ(lines.size(), expectedLines.size());
    EXPECT_EQ(lines, expectedLines);
}

TEST(TestStringTools, splitIntoLines_Do_not_apply_wordWrap_on_long_line_Return_single_line)
{
    const std::vector<std::string> expectedLines { "12345678" };
    const std::string inputText = "12345678";

    auto lines = splitIntoLines(inputText, 5);
    EXPECT_EQ(lines.size(), expectedLines.size());
    EXPECT_EQ(lines, expectedLines);
}

TEST(TestStringTools, splitIntoLines_Multiple_long_lines_No_wordWrap_Do_not_split_the_lines)
{
    std::vector<std::string> expectedLines { "123456789", "uvwx", "abcdefghijkl" };
    const auto inputText = expectedLines[0] + "\n" + expectedLines[1] + "\n" + expectedLines[2];

    auto lines = splitIntoLines(inputText, 5);
    EXPECT_EQ(lines.size(), expectedLines.size());
    EXPECT_EQ(lines, expectedLines);
}

TEST(TestStringTools, splitIntoLines_Apply_wordWrap_on_multiple_long_lines)
{
    std::vector<std::string> expectedLines { "12345", "6789", "uvwx", "abcde", "fghij", "kl" };
    const auto inputText = expectedLines[0] + expectedLines[1] + "\n"
            + expectedLines[2] + "\n"
            + expectedLines[3] + expectedLines[4] + expectedLines[5];

    auto lines = splitIntoLines(inputText, 5, true);
    EXPECT_EQ(lines.size(), expectedLines.size());
    EXPECT_EQ(lines, expectedLines);
}

TEST(TestStringTools, splitIntoLines_Line_length_equals_max_width_Return_single_line)
{
    std::vector<std::string> expectedLines { "12345" };
    const std::string inputText = "12345";

    auto lines = splitIntoLines(inputText, 5);
    EXPECT_EQ(lines.size(), expectedLines.size());
    EXPECT_EQ(lines, expectedLines);
}

TEST(TestStringTools, splitIntoLines_Line_length_equals_max_width_with_LineFeed_at_the_end_Return_the_text_on_first_line_and_an_empty_line)
{
    std::vector<std::string> expectedLines { "12345", "" };
    const std::string inputText = expectedLines.front() + "\n";

    auto lines = splitIntoLines(inputText, 5);
    EXPECT_EQ(lines.size(), expectedLines.size());
    EXPECT_EQ(lines, expectedLines);
}

TEST(TestStringTools, splitIntoLines_Without_wordWrap_For_non_empty_text_and_0_available_width_Return_unsplit_lines)
{
    const std::vector<std::string> expectedLines {
        "Some extra long text that would have otherwise had to be wrapped on a new line.",
        "This text is deliberately placed on a new line.",
        ""
    };
    const auto inputText = expectedLines[0] + "\n" + expectedLines[1] +"\n";
    auto lines = splitIntoLines(inputText, 0);

    EXPECT_EQ(lines.size(), expectedLines.size());
    EXPECT_EQ(lines, expectedLines);
}

TEST(TestStringTools, splitIntoLines_With_wordWrap_For_non_empty_text_and_0_available_width_Return_unsplit_lines)
{
    const std::vector<std::string> expectedLines {
        "Some extra long text that would have otherwise had to be wrapped on a new line.",
        "This text is deliberately placed on a new line.",
        ""
    };
    const auto inputText = expectedLines[0] + "\n" + expectedLines[1] +"\n";
    auto lines = splitIntoLines(inputText, 0, true);

    EXPECT_EQ(lines.size(), expectedLines.size());
    EXPECT_EQ(lines, expectedLines);
}
