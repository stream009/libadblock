#include "type.hpp"
#include "rule/comment_rule.hpp"

#include <boost/lexical_cast.hpp>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Rule_CommentRule, Basic)
{
    const auto &comment = "!comment"_r;
    const CommentRule rule { comment };

    EXPECT_EQ(comment, boost::lexical_cast<std::string>(rule));
}
