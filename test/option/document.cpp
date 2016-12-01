#include "option/document_option.hpp"
#include "../mock_context.hpp"

#include "filter_rule_base.hpp"
#include "parser/parser.hpp"
#include "rule/filter_rule.hpp"
#include "pattern/pattern.hpp"

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

TEST(Document, Elementary)
{
    FilterRuleBase rb;

    const auto &rule1 =
        std::dynamic_pointer_cast<FilterRule>(parser::parse("adblock"_r));
    assert(rule1);
    rb.put(*rule1);

    TestContext cxt { "http://www.adblock.org"_u };

    { // suppose to be blocked by rule1
        const auto &rv = rb.query("http://www.adblock.org"_u, cxt);
        ASSERT_TRUE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule1));
    }

    const auto &disabler =
        std::dynamic_pointer_cast<FilterRule>(
                            parser::parse("@@||adblock.org$document"_r));
    assert(disabler);
    rb.put(*disabler);

    { // match with rule1 but be excluded by disabler
        const auto &rv = rb.query("http://www.adblock.org"_u, cxt);
        ASSERT_FALSE(rv.first);
        EXPECT_TRUE(rv.second == &(*disabler));
    }

    { // This won't be excluded because origin doesn't match with disabler.
        TestContext cxt { "http://www.google.com"_u };

        const auto &rv = rb.query("http://www.adblock.org"_u, cxt);
        ASSERT_TRUE(rv.first) << *rv.second;
        EXPECT_TRUE(rv.second == &(*rule1));
    }
}

TEST(Document, Inversed)
{
    FilterRuleBase rb;

    const auto &rule1 =
        std::dynamic_pointer_cast<FilterRule>(parser::parse("adblock"_r));
    assert(rule1);
    rb.put(*rule1);

    TestContext cxt { "http://www.adblock.org"_u };

    { // suppose to be blocked by rule1
        const auto &rv = rb.query("http://www.adblock.org"_u, cxt);
        ASSERT_TRUE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule1));
    }

    const auto &disabler =
        std::dynamic_pointer_cast<FilterRule>(
                            parser::parse("@@||adblock.org$~document"_r));
    assert(disabler);
    rb.put(*disabler);

    { // inverted document option should just be ignored
        const auto &rv = rb.query("http://www.adblock.org"_u, cxt);
        ASSERT_TRUE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule1));
    }

}

} // namespace adblock
