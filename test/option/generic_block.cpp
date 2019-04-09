#include "option/generic_block_option.hpp"

#include "../mock_context.hpp"
#include "../parse_rule.hpp"

#include "filter_rule_base.hpp"
#include "rule/filter_rule.hpp"

#include <gtest/gtest.h>

namespace adblock {

struct TestContext : MockContext
{
    TestContext(const Uri &uri)
        : m_uri { uri }
    {}

    const Uri &origin() const override { return m_uri; }

    Uri m_uri;
};

TEST(Option_GenericBlock, Elementary)
{
    FilterRuleBase rb;

    auto const rule1 = parse_rule<FilterRule>("generic"_r);
    ASSERT_TRUE(rule1);

    rb.put(*rule1);

    TestContext cxt { "http://www.adblock.org"_u };

    { // suppose to be blocked by rule "generic"
        auto const& rv = rb.query("http://www.generic-url.com"_u, cxt);
        ASSERT_TRUE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule1));
    }

    auto const disabler = parse_rule<FilterRule>("@@||adblock.org$genericblock"_r);
    ASSERT_TRUE(disabler);

    rb.put(*disabler);

    { // match with rule "generic" but be excluded by genricblock rule
        auto const& rv = rb.query("http://www.generic-url.com"_u, cxt);
        ASSERT_FALSE(rv.first) << *rv.second;
        EXPECT_TRUE(rv.second == nullptr);
    }
}

TEST(Option_GenericBlock, OnlyGenericShouldBeHidden)
{
    FilterRuleBase rb;

    auto const rule1 = parse_rule<FilterRule>("generic"_r);
    ASSERT_TRUE(rule1);

    rb.put(*rule1);

    auto const rule2 = parse_rule<FilterRule>("generic$domain=adblock.org"_r);
    ASSERT_TRUE(rule2);

    rb.put(*rule2);

    TestContext cxt { "http://www.adblock.org"_u };

    { // suppose to be blocked
        auto const& rv = rb.query("http://www.generic-url.com"_u, cxt);
        ASSERT_TRUE(rv.first);
    }

    auto const disabler = parse_rule<FilterRule>("@@||adblock.org$genericblock"_r);
    ASSERT_TRUE(disabler);

    rb.put(*disabler);

    { // rule1 should be excluded by genericblock but rule2 should remain in effect.
        auto const& rv = rb.query("http://www.generic-url.com"_u, cxt);
        EXPECT_TRUE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule2)) << *rv.second;
    }
}

} // namespace adblock
