#include "../mock_context.hpp"
#include "../parse_rule.hpp"

#include "type.hpp"
#include "rule/filter_rule.hpp"

#include <string>

#include <gtest/gtest.h>

namespace adblock {

struct SiteKeyContext : MockContext
{
    SiteKeyContext(StringRange const& siteKey)
        : m_siteKey { siteKey.begin(), siteKey.end() }
    {}

    StringRange siteKey() const override
    {
        return { m_siteKey.data(), m_siteKey.data() + m_siteKey.size() };
    }

    std::string m_siteKey;
};

TEST(Option_SiteKeyOption, Match)
{
    auto const rule = parse_rule<FilterRule>(
        "whatever$sitekey=abcdsitekeydcba|bcdesitekeyedcb"_r);
    ASSERT_TRUE(rule);

    auto const& uri = "http://whatever.idontcare.com"_u;
    SiteKeyContext const context { "bcdesitekeyedcb"_r };

    EXPECT_TRUE(rule->match(uri, context));
}

TEST(Option_SiteKeyOption, NoMatch)
{
    auto const rule = parse_rule<FilterRule>(
        "whatever$sitekey=abcdsitekeydcba|bcdesitekeyedcb"_r);
    ASSERT_TRUE(rule);

    auto const& uri = "http://whatever.idontcare.com"_u;
    SiteKeyContext const context { "foobarxyzzy"_r };

    EXPECT_FALSE(rule->match(uri, context));
}

} // namespace adblock
