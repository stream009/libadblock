#include "mock_context.hpp"

#include "filter_rule_base.hpp"
#include "type.hpp"
#include "pattern/basic_match_pattern.hpp"
#include "pattern/domain_match_pattern.hpp"
#include "pattern/regex_pattern.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/exception_filter_rule.hpp"

#include <memory>

#include <boost/optional.hpp>

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
                 const boost::optional<StringRange> &pattern)
{
    return std::make_shared<R>(
            std::make_shared<DomainMatchPattern>(domain, pattern, false),
            boost::none);
}

TEST(FilterRuleBase, Basic)
{
    FilterRuleBase rb;
    MockContext cxt;

    rb.put(make_rule<BasicFilterRule, BasicMatchPattern>("adblock"_r));

    EXPECT_TRUE(rb.query("http://www.adblockplug.org"_u, cxt));
}

TEST(FilterRuleBase, Domain)
{
    FilterRuleBase rb;
    MockContext cxt;

    rb.put(make_domain_rule<BasicFilterRule>("adblock.org"_r, boost::none));

    EXPECT_TRUE(rb.query("http://www.adblock.org"_u, cxt));
}

TEST(FilterRuleBase, Regex)
{
    FilterRuleBase rb;
    MockContext cxt;

    rb.put(make_rule<BasicFilterRule, RegexPattern>(".*adblock.*"_r));

    EXPECT_TRUE(rb.query("http://www.adblock.org"_u, cxt));
}

TEST(FilterRuleBase, ExceptionBasic)
{
    FilterRuleBase rb;
    MockContext cxt;

    rb.put(make_rule<BasicFilterRule, BasicMatchPattern>("adblock"_r));
    rb.put(
        make_rule<ExceptionFilterRule, BasicMatchPattern>("adblockplus"_r));

    // match with basic but excluded by exception rule
    EXPECT_FALSE(rb.query("http://www.adblockplus.org"_u, cxt));
}

TEST(FilterRuleBase, ExceptionDomain)
{
    FilterRuleBase rb;
    MockContext cxt;

    rb.put(make_rule<BasicFilterRule, BasicMatchPattern>("adblock"_r));
    rb.put(make_domain_rule<ExceptionFilterRule>("adblock.org"_r, boost::none));

    // match with basic but excluded by exception rule
    EXPECT_FALSE(rb.query("http://www.adblock.org"_u, cxt));
}

TEST(FilterRuleBase, ExceptionRegex)
{
    FilterRuleBase rb;
    MockContext cxt;

    rb.put(make_rule<BasicFilterRule, BasicMatchPattern>("adblock"_r));
    rb.put(make_rule<ExceptionFilterRule,
                            RegexPattern>(R"(.*adblock\.org.*)"_r));

    // match with basic but excluded by exception rule
    EXPECT_FALSE(rb.query("http://www.adblock.org"_u, cxt));
}
