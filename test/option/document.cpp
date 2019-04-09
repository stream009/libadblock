#include "option/document_option.hpp"

#include "../mock_context.hpp"
#include "../parse_rule.hpp"

#include "filter_rule_base.hpp"
#include "rule/filter_rule.hpp"
#include "pattern/pattern.hpp"

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

TEST(Option_Document, Elementary)
{
    FilterRuleBase rb;

    auto const rule1 = parse_rule<FilterRule>("adblock"_r);
    ASSERT_TRUE(rule1);

    rb.put(*rule1);

    TestContext cxt { "http://www.adblock.org"_u };

    { // suppose to be blocked by rule1
        auto const& rv = rb.query("http://www.adblock.org"_u, cxt);
        ASSERT_TRUE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule1));
    }

    auto const disabler = parse_rule<FilterRule>("@@||adblock.org$document"_r);
    ASSERT_TRUE(disabler);

    rb.put(*disabler);

    { // match with rule1 but be excluded by disabler
        auto const& rv = rb.query("http://www.adblock.org"_u, cxt);
        ASSERT_FALSE(rv.first);
        EXPECT_TRUE(rv.second == &(*disabler));
    }

    { // This won't be excluded because origin doesn't match with disabler.
        TestContext cxt { "http://www.google.com"_u };

        auto const& rv = rb.query("http://www.adblock.org"_u, cxt);
        ASSERT_TRUE(rv.first) << *rv.second;
        EXPECT_TRUE(rv.second == &(*rule1));
    }
}

TEST(Option_Document, Inversed)
{
    FilterRuleBase rb;

    auto const rule1 = parse_rule<FilterRule>("adblock"_r);
    ASSERT_TRUE(rule1);

    rb.put(*rule1);

    TestContext cxt { "http://www.adblock.org"_u };

    { // suppose to be blocked by rule1
        auto const& rv = rb.query("http://www.adblock.org"_u, cxt);
        ASSERT_TRUE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule1));
    }

    auto const disabler = parse_rule<FilterRule>("@@||adblock.org$~document"_r);
    ASSERT_TRUE(disabler);

    rb.put(*disabler);

    { // inverted document option should just be ignored
        auto const& rv = rb.query("http://www.adblock.org"_u, cxt);
        ASSERT_TRUE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule1));
    }
}

} // namespace adblock
