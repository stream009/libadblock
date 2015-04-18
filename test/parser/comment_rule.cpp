#include "parser/grammar.hpp"
#include "rule/comment_rule.hpp"

#include <memory>

#include <boost/lexical_cast.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/spirit/include/qi.hpp>

#include <gtest/gtest.h>

using namespace adblock;
namespace qi = boost::spirit::qi;

const static parser::Grammar grammar;

TEST(CommentRuleParser, Basic)
{
    const auto &line = boost::as_literal("! comment line");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv && it == line.end()) << *it;
    const auto &rule = std::dynamic_pointer_cast<CommentRule>(result);
    EXPECT_TRUE(!!rule);

    EXPECT_EQ("! comment line", boost::lexical_cast<std::string>(*rule));
}

