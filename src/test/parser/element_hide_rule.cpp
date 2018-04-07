#include "parser/element_hide_rule.hpp"
#include "rule/basic_element_hide_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"
#include "rule/extended_element_hide_rule.hpp"

#include <memory>

#include <boost/lexical_cast.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/spirit/include/qi.hpp>

#include <gtest/gtest.h>

using namespace adblock;
namespace qi = boost::spirit::qi;

const static parser::ElementHideRule grammar;

TEST(ElementHideRuleParser, Basic)
{
    const auto &line = boost::as_literal("###ad");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv && it == line.end());
    const auto &rule = std::dynamic_pointer_cast<BasicElementHideRule>(result);
    EXPECT_TRUE(!!rule);

    EXPECT_TRUE(boost::equals("#ad", rule->cssSelector()));

    EXPECT_EQ(0, rule->includeDomains().size());
    EXPECT_EQ(0, rule->excludeDomains().size());
}

TEST(ElementHideRuleParser, SingleDomain1)
{
    const auto &line = boost::as_literal("adblock.org###ad");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv && it == line.end());
    const auto &rule = std::dynamic_pointer_cast<BasicElementHideRule>(result);
    EXPECT_TRUE(!!rule);

    EXPECT_TRUE(boost::equals("#ad", rule->cssSelector()));

    const auto &domains = rule->includeDomains();
    EXPECT_EQ(1, domains.size());
    EXPECT_TRUE(boost::equals("adblock.org", domains[0]));

    EXPECT_EQ(0, rule->excludeDomains().size());
}

TEST(ElementHideRuleParser, SingleDomain2)
{
    const auto &line = boost::as_literal("~adblock.org###ad");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv && it == line.end());
    const auto &rule = std::dynamic_pointer_cast<BasicElementHideRule>(result);
    EXPECT_TRUE(!!rule);

    EXPECT_TRUE(boost::equals("#ad", rule->cssSelector()));

    EXPECT_EQ(0, rule->includeDomains().size());

    const auto &domains = rule->excludeDomains();
    EXPECT_EQ(1, domains.size());
    EXPECT_TRUE(boost::equals("adblock.org", domains[0])) << domains[0];
}

TEST(ElementHideRuleParser, MultipleDomain)
{
    const auto &line =
              boost::as_literal("adblock.org,~google.com,facebook.com###ad");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv && it == line.end());
    const auto &rule = std::dynamic_pointer_cast<BasicElementHideRule>(result);
    EXPECT_TRUE(!!rule);

    EXPECT_TRUE(boost::equals("#ad", rule->cssSelector()));

    auto &&domains = rule->includeDomains();
    EXPECT_EQ(2, domains.size());
    EXPECT_TRUE(boost::equals("adblock.org", domains[0]));
    EXPECT_TRUE(boost::equals("facebook.com", domains[1]));

    domains = rule->excludeDomains();
    EXPECT_EQ(1, domains.size());
    EXPECT_TRUE(boost::equals("google.com", domains[0]));
}

TEST(ElementHideRuleParser, WrongDomain1)
{
    const auto &line =
              boost::as_literal("ad|block.org###ad");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    EXPECT_FALSE(rv);
}

TEST(ElementHideRuleParser, WrongDomain2)
{
    const auto &line =
              boost::as_literal("adblock.org,###ad");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    EXPECT_FALSE(rv);
}

TEST(ElementHideRuleParser, WrongDomain3)
{
    const auto &line =
              boost::as_literal(",adblock.org###ad");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    EXPECT_FALSE(rv);
}

TEST(ElementHideRuleParser, WrongDomain4)
{
    const auto &line =
              boost::as_literal("adblock.org, google.com###ad");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    EXPECT_FALSE(rv);
}

TEST(ExceptionElementHideRuleParser, Basic)
{
    const auto &line = boost::as_literal("#@##ad");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv && it == line.end());
    const auto &rule =
            std::dynamic_pointer_cast<ExceptionElementHideRule>(result);
    EXPECT_TRUE(!!rule);

    EXPECT_TRUE(boost::equals("#ad", rule->cssSelector()));

    EXPECT_EQ(0, rule->includeDomains().size());
    EXPECT_EQ(0, rule->excludeDomains().size());
}

TEST(ExceptionElementHideRuleParser, Wrong1)
{
    const auto &line = boost::as_literal("#@@##ad");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    EXPECT_FALSE(rv);
}

TEST(ExceptionElementHideRuleParser, Wrong2)
{
    const auto &line = boost::as_literal("@##ad");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    EXPECT_FALSE(rv);
}

// ExtendedElementHideRule

TEST(ExtendedElementHideRuleParser, Basic)
{
    auto const& line = boost::as_literal("#?##ad");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    auto const rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv && it == line.end());
    auto const& rule =
            std::dynamic_pointer_cast<ExtendedElementHideRule>(result);
    EXPECT_TRUE(!!rule);

    EXPECT_TRUE(boost::equals("#ad", rule->cssSelector()));

    EXPECT_EQ(0, rule->includeDomains().size());
    EXPECT_EQ(0, rule->excludeDomains().size());
}

TEST(ExtendedElementHideRuleParser, Wrong1)
{
    auto const& line = boost::as_literal("#??##ad");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    auto const rv = qi::parse(it, line.end(), grammar, result);

    EXPECT_FALSE(rv);
}

TEST(ExtendedElementHideRuleParser, Wrong2)
{
    auto const& line = boost::as_literal("?##ad");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    auto const rv = qi::parse(it, line.end(), grammar, result);

    EXPECT_FALSE(rv);
}

