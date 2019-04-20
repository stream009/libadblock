#include "../mock_context.hpp"
#include "../parse_rule.hpp"

#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "rule/filter_rule.hpp"

#include <gtest/gtest.h>

namespace adblock {

struct XmlHttpRequestContext : MockContext
{
    XmlHttpRequestContext(bool const flag)
        : m_flag { flag }
    {}

    bool fromXmlHttpRequest() const override { return m_flag; }

    bool m_flag;
};

TEST(Option_XmlHttpRequestOption, XmlHttpRequestContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$xmlhttprequest"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/data.json" };
    XmlHttpRequestContext const context { true };

    EXPECT_TRUE(rule->match(uri, context));
}

TEST(Option_XmlHttpRequestOption, NotXmlHttpRequestContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$xmlhttprequest"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/data.json" };
    XmlHttpRequestContext const context { false };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_XmlHttpRequestOption, XmlHttpRequestContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~xmlhttprequest"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/data.json" };
    XmlHttpRequestContext const context { true };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_XmlHttpRequestOption, NotXmlHttpRequestContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~xmlhttprequest"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/data.json" };
    XmlHttpRequestContext const context { false };

    EXPECT_TRUE(rule->match(uri, context));
}

} // namespace adblock
