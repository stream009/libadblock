#include "../mock_context.hpp"
#include "../parse_rule.hpp"

#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "rule/filter_rule.hpp"

#include <gtest/gtest.h>

namespace adblock {

struct DomainContext : MockContext
{
    DomainContext(const Uri &uri)
        : m_uri { uri }
    {}

    const Uri &origin() const override { return m_uri; }

    Uri m_uri;
};

TEST(Option_DomainOption, Match)
{
    auto const rule = parse_rule<FilterRule>("image.jpg$domain=adblock.org"_r);
    ASSERT_TRUE(rule);

    auto const& uri = "http://www.google.com/image.jpg"_u;
    DomainContext const context { "http://www.adblock.org/"_u };

    EXPECT_TRUE(rule->match(uri, context));
}

TEST(Option_DomainOption, NoMatch)
{
    auto const rule = parse_rule<FilterRule>("image.jpg$domain=adblock.org"_r);
    ASSERT_TRUE(rule);

    auto const& uri = "http://www.google.com/image.jpg"_u;
    DomainContext const context { "http://www.google.com/"_u };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_DomainOption, MatchToNegative)
{
    auto const rule = parse_rule<FilterRule>(
                        "image.jpg$domain=adblock.org|~sub.adblock.org"_r);
    ASSERT_TRUE(rule);

    auto const& uri = "http://www.google.com/image.jpg"_u;

    DomainContext const context1 { "http://www.sub.adblock.org/"_u };
    EXPECT_FALSE(rule->match(uri, context1));

    DomainContext const context2 { "http://www.adblock.org/"_u };
    EXPECT_TRUE(rule->match(uri, context2));
}

TEST(Option_DomainOption, ExcludeDomainOnlyOption)
{
    auto const rule = parse_rule<FilterRule>("image.jpg$domain=~adblock.org"_r);
    ASSERT_TRUE(rule);

    auto const& uri = "http://www.google.com/image.jpg"_u;
    DomainContext const context { "http://www.google.com/"_u };

    EXPECT_TRUE(rule->match(uri, context));
}

} // namespace adblock
