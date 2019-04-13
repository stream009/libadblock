#include "parser/parser.hpp"

#include "rule/filter_option.hpp"
#include "rule/filter_rule.hpp"
#include "type.hpp"

#include <memory>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Parser_WhiteListOption, Document_Normal)
{
    auto const& line = "@@adblock$document"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::Document));
}

TEST(Parser_WhiteListOption, Document_Inverse)
{
    auto const& line = "@@adblock$~document"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::DocumentInv));
}

TEST(Parser_WhiteListOption, ElemHide_Normal)
{
    auto const& line = "@@adblock$elemhide"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::ElemHide));
}

TEST(Parser_WhiteListOption, ElemHide_Inverse)
{
    auto const& line = "@@adblock$~elemhide"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::ElemHideInv));
}

TEST(Parser_WhiteListOption, GenericBlock)
{
    auto const& line = "@@adblock$genericblock"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());

    EXPECT_TRUE(filter->hasOption(FilterOption::GenericBlock));
}

TEST(Parser_WhiteListOption, GenericHide)
{
    auto const& line = "@@adblock$generichide"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());

    EXPECT_TRUE(filter->hasOption(FilterOption::GenericHide));
}
