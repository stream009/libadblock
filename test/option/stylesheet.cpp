#include "../mock_context.hpp"
#include "../parse_rule.hpp"

#include "rule/filter_rule.hpp"

#include <gtest/gtest.h>

namespace adblock {

struct StyleSheetContext : MockContext
{
    StyleSheetContext(const bool flag)
        : m_flag { flag }
    {}

    bool isExternalStyleSheet() const override { return m_flag; }

    bool m_flag;
};

TEST(Option_StyleSheetOption, StyleSheetContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$stylesheet"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/stylesheet.css" };
    StyleSheetContext const context { true };

    EXPECT_TRUE(rule->match(uri, context));
}

TEST(Option_StyleSheetOption, NotStyleSheetContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$stylesheet"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/stylesheet.css" };
    StyleSheetContext const context { false };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_StyleSheetOption, StyleSheetContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~stylesheet"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/stylesheet.css" };
    StyleSheetContext const context { true };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_StyleSheetOption, NotStyleSheetContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~stylesheet"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/stylesheet.css" };
    StyleSheetContext const context { false };

    EXPECT_TRUE(rule->match(uri, context));
}

} // namespace adblock
