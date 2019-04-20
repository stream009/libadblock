#include "../mock_context.hpp"
#include "../parse_rule.hpp"

#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "rule/filter_rule.hpp"

#include <gtest/gtest.h>

namespace adblock {

struct FontContext : MockContext
{
    FontContext(bool const flag)
        : m_flag { flag }
    {}

    bool isFont() const override { return m_flag; }

    bool m_flag;
};

TEST(Option_FontOption, FontContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$font"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/font.tff" };
    FontContext const context { true };

    EXPECT_TRUE(rule->match(uri, context));
}

TEST(Option_FontOption, NotFontContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$font"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/font.tff" };
    FontContext const context { false };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_FontOption, FontContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~font"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/font.tff" };
    FontContext const context { true };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_FontOption, NotFontContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~font"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/font.tff" };
    FontContext const context { false };

    EXPECT_TRUE(rule->match(uri, context));
}

} // namespace adblock
