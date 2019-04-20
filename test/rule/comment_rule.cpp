#include "core/string_range.hpp"
#include "rule/comment_rule.hpp"

#include <boost/lexical_cast.hpp>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Rule_CommentRule, Basic)
{
    auto const comment = "!comment"_r;
    CommentRule const rule { comment };

    EXPECT_EQ(comment, boost::lexical_cast<std::string>(rule));
}
