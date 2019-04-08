#include "parser/parser.hpp"

#include "option.hpp"
#include "option/document_option.hpp"
#include "option/elem_hide_option.hpp"
#include "option/generic_block_option.hpp"
#include "option/generic_hide_option.hpp"
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

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<DocumentOption const*>(options.front());
    ASSERT_TRUE(option);
    EXPECT_FALSE(option->inverse());
}

TEST(Parser_WhiteListOption, Document_Inverse)
{
    auto const& line = "@@adblock$~document"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<DocumentOption const*>(options.front());
    ASSERT_TRUE(option);
    EXPECT_TRUE(option->inverse());
}

TEST(Parser_WhiteListOption, ElemHide_Normal)
{
    auto const& line = "@@adblock$elemhide"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<ElemHideOption const*>(options.front());
    ASSERT_TRUE(option);
    EXPECT_FALSE(option->inverse());
}

TEST(Parser_WhiteListOption, ElemHide_Inverse)
{
    auto const& line = "@@adblock$~elemhide"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<ElemHideOption const*>(options.front());
    ASSERT_TRUE(option);
    EXPECT_TRUE(option->inverse());
}

TEST(Parser_WhiteListOption, GenericBlock)
{
    auto const& line = "@@adblock$genericblock"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<GenericBlockOption const*>(options.front());
    ASSERT_TRUE(option);
}

TEST(Parser_WhiteListOption, GenericHide)
{
    auto const& line = "@@adblock$generichide"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<GenericHideOption const*>(options.front());
    ASSERT_TRUE(option);
}
