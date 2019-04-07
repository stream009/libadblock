#include "option/other_option.hpp"
#include "../mock_context.hpp"

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

const static OtherOption option { false };
const static OtherOption optionInv { true };

TEST(OtherOption, Constructor)
{
    EXPECT_FALSE(option.inverse());
    EXPECT_TRUE(optionInv.inverse());
}

TEST(OtherOption, OtherContext)
{
    const Uri uri { "http://adblock.org/image.jpg" };
    const OtherContext context { true };

    EXPECT_TRUE(option.match(uri, context));
}

TEST(OtherOption, NotOtherContext)
{
    const Uri uri { "http://adblock.org/script.js" };
    const OtherContext context { false };

    EXPECT_FALSE(option.match(uri, context));
}

TEST(OtherOption, OtherContextWithInverseOption)
{
    const Uri uri { "http://adblock.org/image.jpg" };
    const OtherContext context { true };

    EXPECT_FALSE(optionInv.match(uri, context));
}

TEST(OtherOption, NotOtherContextWithInverseOption)
{
    const Uri uri { "http://adblock.org/script.js" };
    const OtherContext context { false };

    EXPECT_TRUE(optionInv.match(uri, context));
}

} // namespace adblock
