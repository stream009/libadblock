#include "../mock_context.hpp"
#include "../parse_rule.hpp"

#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "rule/filter_rule.hpp"

#include <gtest/gtest.h>

namespace adblock {

struct ImageContext : MockContext
{
    ImageContext(bool const flag)
        : m_flag { flag }
    {}

    bool fromImageTag() const override { return m_flag; }

    bool m_flag;
};

TEST(Option_ImageOption, ImageContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$image"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/image.jpg" };
    ImageContext const context { true };

    EXPECT_TRUE(rule->match(uri, context));
}

TEST(Option_ImageOption, NotImageContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$image"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/image.jpg" };
    ImageContext const context { false };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_ImageOption, ImageContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~image"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/image.jpg" };
    ImageContext const context { true };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_ImageOption, NotImageContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~image"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/image.jpg" };
    ImageContext const context { false };

    EXPECT_TRUE(rule->match(uri, context));
}

} // namespace adblock
