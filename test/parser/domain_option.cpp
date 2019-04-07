#include "parser/parser.hpp"

#include "option.hpp"
#include "option/domain_option.hpp"
#include "rule/filter_rule.hpp"
#include "type.hpp"

#include <memory>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Parser_FilterOption, Domain_Basic)
{
    auto const& line = "adblock$domain=adblock.org"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<DomainOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_EQ(1, option->includeDomains().size());
    EXPECT_EQ(0, option->excludeDomains().size());
    EXPECT_EQ("adblock.org"_r, option->includeDomains()[0]);
}

TEST(Parser_FilterOption, Domain_ExcludeDomain)
{
    auto const& line = "adblock$domain=~adblock.org"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<DomainOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_EQ(0, option->includeDomains().size());
    EXPECT_EQ(1, option->excludeDomains().size());
    EXPECT_EQ("adblock.org"_r, option->excludeDomains()[0]);
}

TEST(Parser_FilterOption, Domain_MultipleDomain1)
{
    auto const& line = "adblock$domain=adblock.org|google.com"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<DomainOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_EQ(2, option->includeDomains().size());
    EXPECT_EQ(0, option->excludeDomains().size());
    EXPECT_EQ("adblock.org"_r, option->includeDomains()[0]);
    EXPECT_EQ("google.com"_r, option->includeDomains()[1]);
}

TEST(Parser_FilterOption, Domain_MultipleDomain2)
{
    auto const& line = "adblock$domain=adblock.org|~google.com"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<DomainOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_EQ(1, option->includeDomains().size());
    EXPECT_EQ(1, option->excludeDomains().size());
    EXPECT_EQ("adblock.org"_r, option->includeDomains()[0]);
    EXPECT_EQ("google.com"_r, option->excludeDomains()[0]);
}
