#include "../mock_context.hpp"
#include "../parse_rule.hpp"

#include "rule/filter_rule.hpp"

#include <gtest/gtest.h>

namespace adblock {

struct ThirdPartyContext : MockContext
{
    ThirdPartyContext(Uri const& uri)
        : m_uri { uri }
    {}

    Uri const& origin() const override { return m_uri; }

    Uri const& m_uri;
};

TEST(Option_ThirdPartyOption, ThirdPartyContext)
{
    auto const rule = parse_rule<FilterRule>("image$third-party"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/image.jpg" };
    Uri const origin { "http://www.google.com" };

    ThirdPartyContext const context { origin };

    EXPECT_TRUE(rule->match(uri, context));
}

TEST(Option_ThirdPartyOption, NotThirdPartyContext)
{
    auto const rule = parse_rule<FilterRule>("image$third-party"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/image.jpg" };
    Uri const origin { "http://adblock.org" };

    ThirdPartyContext const context { origin };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_ThirdPartyOption, NotThirdPartyContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("image$~third-party"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/image.jpg" };
    Uri const origin { "http://www.google.com" };

    ThirdPartyContext const context { origin };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_ThirdPartyOption, ThirdPartyContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("image$~third-party"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/image.jpg" };
    Uri const origin { "http://adblock.org" };

    ThirdPartyContext const context { origin };

    EXPECT_TRUE(rule->match(uri, context));
}

} // namespace adblock
