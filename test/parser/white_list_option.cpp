#include "../parse_rule.hpp"

#include "core/string_range.hpp"
#include "rule/filter_option.hpp"
#include "rule/filter_rule.hpp"

#include <memory>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Parser_WhiteListOption, Document_Normal)
{
    auto const& line = "@@adblock$document"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::Document));
}

TEST(Parser_WhiteListOption, Document_Inverse)
{
    auto const& line = "@@adblock$~document"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::DocumentInv));
}

TEST(Parser_WhiteListOption, ElemHide_Normal)
{
    auto const& line = "@@adblock$elemhide"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::ElemHide));
}

TEST(Parser_WhiteListOption, ElemHide_Inverse)
{
    auto const& line = "@@adblock$~elemhide"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::ElemHideInv));
}

TEST(Parser_WhiteListOption, GenericBlock)
{
    auto const& line = "@@adblock$genericblock"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::GenericBlock));
}

TEST(Parser_WhiteListOption, GenericHide)
{
    auto const& line = "@@adblock$generichide"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::GenericHide));
}
