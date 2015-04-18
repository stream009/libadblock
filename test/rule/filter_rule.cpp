#include "rule/basic_filter_rule.hpp"
#include "pattern/basic_match_pattern.hpp"

#include <memory>

#include <boost/range/iterator_range.hpp>

#include <gtest/gtest.h>

using namespace adblock;

TEST(BasicFilterRule, Basic)
{
    const auto &pattern = std::make_shared<BasicMatchPattern>(
        boost::as_literal("adblock")
    );
    BasicFilterRule rule { pattern, boost::none };

    EXPECT_EQ(&(*pattern), &(rule.pattern()));
    EXPECT_TRUE(rule.options().empty());

    Uri uri { "http://adblock.org" };
    //rule.match(uri, 0); //TODO implement Context first
    //TODO test Option first
}
