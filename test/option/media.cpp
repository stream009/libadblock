#include "../mock_context.hpp"
#include "../parse_rule.hpp"

#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "rule/filter_rule.hpp"

#include <gtest/gtest.h>

namespace adblock {

struct MediaContext : MockContext
{
    MediaContext(bool const flag)
        : m_flag { flag }
    {}

    bool fromAudioVideoTag() const override { return m_flag; }

    bool m_flag;
};

TEST(Option_MediaOption, MediaContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$media"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/video.mp4" };
    MediaContext const context { true };

    EXPECT_TRUE(rule->match(uri, context));
}

TEST(Option_MediaOption, NotMediaContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$media"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/video.mp4" };
    MediaContext const context { false };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_MediaOption, MediaContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~media"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/video.mp4" };
    MediaContext const context { true };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_MediaOption, NotMediaContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~media"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/video.mp4" };
    MediaContext const context { false };

    EXPECT_TRUE(rule->match(uri, context));
}

} // namespace adblock
