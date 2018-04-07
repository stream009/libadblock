#include "type.hpp"
#include "option.hpp"
#include "rule/comment_rule.hpp"

#include <gtest/gtest.h>

using namespace adblock;

TEST(CommentRule, Basic)
{
    const auto &comment = "!comment"_r;
    const CommentRule rule { comment };

    EXPECT_EQ(comment, boost::lexical_cast<std::string>(rule));
}
