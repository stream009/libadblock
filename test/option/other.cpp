#include "../mock_context.hpp"
#include "../parse_rule.hpp"

#include "rule/filter_rule.hpp"

#include <gtest/gtest.h>

namespace adblock {

struct OtherContext : MockContext
{
    OtherContext(const bool flag)
        : m_flag { flag }
    {}

    bool fromScriptTag() const override { return !m_flag; }
    bool fromImageTag() const override { return !m_flag; }
    bool fromAudioVideoTag() const override { return !m_flag; }
    bool isExternalStyleSheet() const override { return !m_flag; }
    bool fromObjectTag() const override { return !m_flag; }
    bool fromXmlHttpRequest() const override { return !m_flag; }
    bool fromPlugins() const override { return !m_flag; }
    bool isSubDocument() const override { return !m_flag; }

    bool m_flag;
};

TEST(Option_OtherOption, OtherContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$other"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/some.dat" };
    OtherContext const context { true };

    EXPECT_TRUE(rule->match(uri, context));
}

TEST(Option_OtherOption, NotOtherContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$other"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/some.dat" };
    OtherContext const context { false };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_OtherOption, OtherContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~other"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/some.dat" };
    OtherContext const context { true };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_OtherOption, NotOtherContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~other"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/some.dat" };
    OtherContext const context { false };

    EXPECT_TRUE(rule->match(uri, context));
}

} // namespace adblock
