#include "parser/parser.hpp"

#include "rule/filter_rule.hpp"
#include "rule/filter_option.hpp"
#include "type.hpp"

#include <memory>

#include <boost/algorithm/string.hpp>

#include <gtest/gtest.h>

using namespace adblock;

namespace ba = boost::algorithm;

TEST(Parser_FilterOption, Domain_Basic)
{
    auto const& line = "adblock$domain=adblock.org"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    ASSERT_TRUE(filter->hasOption(FilterOption::Domain));

    auto* const domains = filter->domains();
    ASSERT_TRUE(domains);
    ASSERT_EQ(1, domains->size());
    EXPECT_TRUE(ba::equals("adblock.org", (*domains)[0]));
}

TEST(Parser_FilterOption, Domain_ExcludeDomain)
{
    auto const& line = "adblock$domain=~adblock.org"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    ASSERT_TRUE(filter->hasOption(FilterOption::Domain));

    auto* const domains = filter->domains();
    ASSERT_TRUE(domains);
    ASSERT_EQ(1, domains->size());
    EXPECT_TRUE(ba::equals("~adblock.org", (*domains)[0]));
}

TEST(Parser_FilterOption, Domain_MultipleDomain1)
{
    auto const& line = "adblock$domain=adblock.org|google.com"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    ASSERT_TRUE(filter->hasOption(FilterOption::Domain));

    auto* const domains = filter->domains();
    ASSERT_TRUE(domains);
    ASSERT_EQ(2, domains->size());
    EXPECT_TRUE(ba::equals("adblock.org", (*domains)[0]));
    EXPECT_TRUE(ba::equals("google.com", (*domains)[1]));
}

TEST(Parser_FilterOption, Domain_MultipleDomain2)
{
    auto const& line = "adblock$domain=adblock.org|~google.com"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    ASSERT_TRUE(filter->hasOption(FilterOption::Domain));

    auto* const domains = filter->domains();
    ASSERT_TRUE(domains);
    ASSERT_EQ(2, domains->size());
    EXPECT_TRUE(ba::equals("adblock.org", (*domains)[0]));
    EXPECT_TRUE(ba::equals("~google.com", (*domains)[1]));
}
