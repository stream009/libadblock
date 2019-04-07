#include "mock_context.hpp"

#include "filter_rule_base.hpp"
#include "type.hpp"
#include "pattern/basic_match_pattern.hpp"
#include "pattern/domain_match_pattern.hpp"
#include "pattern/regex_pattern.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/exception_filter_rule.hpp"
#include "parser/parser.hpp"

#include <memory>

#include <boost/optional.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <gtest/gtest.h>

using namespace adblock;

template<typename R, typename P>
static std::shared_ptr<FilterRule>
make_rule(const StringRange &pattern)
{
    return std::make_shared<R>(std::make_shared<P>(pattern), boost::none);
}

template<typename R>
static std::shared_ptr<FilterRule>
make_domain_rule(const StringRange &domain,
                 const StringRange &pattern,
                 const boost::optional<
                    std::vector<std::shared_ptr<Option>>> &options = boost::none)
{
    return std::make_shared<R>(
            std::make_shared<DomainMatchPattern>(domain, pattern, false),
            options);
}

TEST(FilterRuleBase, Basic)
{
    FilterRuleBase rb;
    MockContext cxt;

    const auto &rule
        = make_rule<BasicFilterRule, BasicMatchPattern>("adblock"_r);
    assert(rule);
    rb.put(*rule);

    {
        auto const& rv = rb.query("http://www.adblock.org"_u, cxt);
        ASSERT_TRUE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule));
    }
    {
        auto const& rv = rb.query("http://www.google.com"_u, cxt);
        ASSERT_FALSE(rv.first);
        EXPECT_TRUE(rv.second == nullptr);
    }
}

TEST(FilterRuleBase, Domain)
{
    FilterRuleBase rb;
    MockContext cxt;

    const auto &rule
        = make_domain_rule<BasicFilterRule>("adblock.org"_r, ""_r);
    assert(rule);
    rb.put(*rule);

    {
        auto const& rv = rb.query("http://www.adblock.org"_u, cxt);
        ASSERT_TRUE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule));
    }
    {
        auto const& rv = rb.query("http://www.google.com"_u, cxt);
        ASSERT_FALSE(rv.first);
        EXPECT_TRUE(rv.second == nullptr);
    }
}

TEST(FilterRuleBase, Regex)
{
    FilterRuleBase rb;
    MockContext cxt;

    const auto &rule
        = make_rule<BasicFilterRule, RegexPattern>(".*adblock.*"_r);
    assert(rule);
    rb.put(*rule);

    {
        auto const& rv = rb.query("http://www.adblock.org"_u, cxt);
        ASSERT_TRUE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule));
    }
    {
        auto const& rv = rb.query("http://www.google.com"_u, cxt);
        ASSERT_FALSE(rv.first);
        EXPECT_TRUE(rv.second == nullptr);
    }
}

TEST(FilterRuleBase, ExceptionBasic)
{
    FilterRuleBase rb;
    MockContext cxt;

    const auto &rule1
        = make_rule<BasicFilterRule, BasicMatchPattern>("org"_r);
    assert(rule1);
    rb.put(*rule1);

    const auto &rule2 =
        make_rule<ExceptionFilterRule, BasicMatchPattern>("adblock"_r);
    assert(rule2);
    rb.put(*rule2);

    {
        auto const& rv = rb.query("http://www.adblock.org"_u, cxt);
        ASSERT_FALSE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule2));
    }
    {
        auto const& rv = rb.query("http://www.foobar.org"_u, cxt);
        EXPECT_TRUE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule1)) << rv.second;
    }
}

TEST(FilterRuleBase, ExceptionDomain)
{
    FilterRuleBase rb;
    MockContext cxt;

    const auto &rule1
        = make_rule<BasicFilterRule, BasicMatchPattern>("org"_r);
    assert(rule1);
    rb.put(*rule1);

    const auto &rule2 = make_domain_rule<ExceptionFilterRule>(
                                        "adblock.org"_r, ""_r);
    assert(rule2);
    rb.put(*rule2);

    {
        auto const& rv = rb.query("http://www.adblock.org"_u, cxt);
        ASSERT_FALSE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule2));
    }
    {
        auto const& rv = rb.query("http://www.foobar.org"_u, cxt);
        EXPECT_TRUE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule1)) << rv.second;
    }
}

TEST(FilterRuleBase, ExceptionRegex)
{
    FilterRuleBase rb;
    MockContext cxt;

    const auto &rule1
        = make_rule<BasicFilterRule, BasicMatchPattern>("org"_r);
    assert(rule1);
    rb.put(*rule1);

    const auto &rule2 = make_rule<ExceptionFilterRule,
                            RegexPattern>(R"(.*adblock.*)"_r);
    assert(rule2);
    rb.put(*rule2);

    {
        auto const& rv = rb.query("http://www.adblock.org"_u, cxt);
        ASSERT_FALSE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule2));
    }
    {
        auto const& rv = rb.query("http://www.foobar.org"_u, cxt);
        EXPECT_TRUE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule1)) << rv.second;
    }
}

TEST(FilterRuleBase, Clear)
{
    FilterRuleBase rb;

    auto &&rule =
        make_rule<BasicFilterRule, BasicMatchPattern>("adblock"_r);
    assert(rule);
    rb.put(*rule);

    rule = make_rule<ExceptionFilterRule,
                            RegexPattern>(R"(.*adblock\.org.*)"_r);
    assert(rule);
    rb.put(*rule);

    rule =
        std::dynamic_pointer_cast<FilterRule>(parser::parse("/adsense/*"_r));
    assert(rule);
    rb.put(*rule);

    rule = std::dynamic_pointer_cast<FilterRule>(
                parser::parse("@@||www.google.*/adsense/$~third-party"_r));
    assert(rule);
    rb.put(*rule);

    const auto &before = rb.statistics();
    EXPECT_EQ(4, before.get<size_t>("Total"));
    EXPECT_EQ(2, before.get<size_t>("Basic match pattern"));
    EXPECT_EQ(2, before.get<size_t>("Exception match pattern"));

    rb.clear();

    const auto &after = rb.statistics();
    EXPECT_EQ(0, after.get<size_t>("Total"));
    EXPECT_EQ(0, after.get<size_t>("Basic match pattern"));
    EXPECT_EQ(0, after.get<size_t>("Exception match pattern"));
}
