#include "../parse_rule.hpp"

#include "core/string_range.hpp"
#include "rule/comment_rule.hpp"

#include <memory>

#include <boost/lexical_cast.hpp>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Parser_CommentRule, Basic)
{
    auto const line = "! comment line"_r;

    auto const rule = parse_rule<CommentRule>(line);
    ASSERT_TRUE(rule);

    EXPECT_EQ("! comment line", boost::lexical_cast<std::string>(*rule));
}
