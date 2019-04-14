#include "../mock_context.hpp"
#include "../parse_rule.hpp"

#include "rule/filter_rule.hpp"

#include <gtest/gtest.h>

namespace adblock {

struct ObjectSubRequestContext : MockContext
{
    ObjectSubRequestContext(const bool flag)
        : m_flag { flag }
    {}

    bool fromPlugins() const override { return m_flag; }

    bool m_flag;
};

TEST(Option_ObjectSubRequestOption, ObjectSubRequestContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$object-subrequest"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/data.json" };
    ObjectSubRequestContext const context { true };

    EXPECT_TRUE(rule->match(uri, context));
}

TEST(Option_ObjectSubRequestOption, NotObjectSubRequestContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$object-subrequest"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/data.json" };
    ObjectSubRequestContext const context { false };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_ObjectSubRequestOption, ObjectSubRequestContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~object-subrequest"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/data.json" };
    ObjectSubRequestContext const context { true };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_ObjectSubRequestOption, NotObjectSubRequestContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~object-subrequest"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/data.json" };
    ObjectSubRequestContext const context { false };

    EXPECT_TRUE(rule->match(uri, context));
}

} // namespace adblock
