#include "option/generic_block_option.hpp"
#include "../mock_context.hpp"

#include "filter_rule_base.hpp"
#include "parser/parser.hpp"
#include "rule/filter_rule.hpp"

#include <iostream>

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

TEST(GenericBlock, Elementary)
{
    FilterRuleBase rb;

    const auto &rule1 =
        std::dynamic_pointer_cast<FilterRule>(parser::parse("generic"_r));
    assert(rule1);
    rb.put(*rule1);

    TestContext cxt { "http://www.adblock.org"_u };

    { // suppose to be blocked by rule "generic"
        const auto &rv = rb.query("http://www.generic-url.com"_u, cxt);
        ASSERT_TRUE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule1));
    }

    const auto &disabler =
        std::dynamic_pointer_cast<FilterRule>(
                            parser::parse("@@||adblock.org$genericblock"_r));
    assert(disabler);
    rb.put(*disabler);

    { // match with rule "generic" but be excluded by genricblock rule
        const auto &rv = rb.query("http://www.generic-url.com"_u, cxt);
        ASSERT_FALSE(rv.first) << *rv.second;
        EXPECT_TRUE(rv.second == nullptr);
    }
}

TEST(GenericBlock, OnlyGenericShouldBeHidden)
{
    FilterRuleBase rb;

    const auto &rule1 =
        std::dynamic_pointer_cast<FilterRule>(parser::parse("generic"_r));
    assert(rule1);
    rb.put(*rule1);

    const auto &rule2 =
        std::dynamic_pointer_cast<FilterRule>(
                       parser::parse("generic$domain=adblock.org"_r));
    assert(rule2);
    rb.put(*rule2);

    TestContext cxt { "http://www.adblock.org"_u };

    { // suppose to be blocked
        const auto &rv = rb.query("http://www.generic-url.com"_u, cxt);
        ASSERT_TRUE(rv.first);
    }

    const auto &disabler =
        std::dynamic_pointer_cast<FilterRule>(
                            parser::parse("@@||adblock.org$genericblock"_r));
    assert(disabler);
    rb.put(*disabler);

    { // rule1 should be excluded by genericblock but rule2 should remain in effect.
        const auto &rv = rb.query("http://www.generic-url.com"_u, cxt);
        EXPECT_TRUE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule2)) << *rv.second;
    }
}

} // namespace adblock
