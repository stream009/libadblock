#include "option/media_option.hpp"
#include "type.hpp"
#include "../mock_context.hpp"

#include <gtest/gtest.h>

namespace adblock {

struct MediaContext : MockContext
{
    MediaContext(const bool flag)
        : m_flag { flag }
    {}

    bool fromAudioVideoTag() const override { return m_flag; }

    bool m_flag;
};

const static MediaOption option { false };
const static MediaOption optionInv { true };

TEST(Option_MediaOption, Constructor)
{
    EXPECT_FALSE(option.inverse());
    EXPECT_TRUE(optionInv.inverse());
}

TEST(Option_MediaOption, MediaContext)
{
    const Uri uri { "http://adblock.org/image.jpg" };
    const MediaContext context { true };

    EXPECT_TRUE(option.match(uri, context));
}

TEST(Option_MediaOption, NotMediaContext)
{
    const Uri uri { "http://adblock.org/script.js" };
    const MediaContext context { false };

    EXPECT_FALSE(option.match(uri, context));
}

TEST(Option_MediaOption, MediaContextWithInverseOption)
{
    const Uri uri { "http://adblock.org/image.jpg" };
    const MediaContext context { true };

    EXPECT_FALSE(optionInv.match(uri, context));
}

TEST(Option_MediaOption, NotMediaContextWithInverseOption)
{
    const Uri uri { "http://adblock.org/script.js" };
    const MediaContext context { false };

    EXPECT_TRUE(optionInv.match(uri, context));
}

} // namespace adblock
