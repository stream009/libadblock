#include "parser/parser.hpp"

#include "core/type.hpp"
#include "rule/comment_rule.hpp"

#include <memory>

#include <boost/lexical_cast.hpp>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Parser_CommentRuleParser, Basic)
{
    auto const& line = "! comment line"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto* const comment = dynamic_cast<CommentRule*>(rule.get());
    ASSERT_TRUE(comment);

    EXPECT_EQ("! comment line", boost::lexical_cast<std::string>(*comment));
}
