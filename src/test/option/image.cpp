#include "option/image_option.hpp"
#include "../mock_context.hpp"

#include <boost/spirit/include/qi.hpp>

#include <gtest/gtest.h>

namespace adblock {

namespace qi = boost::spirit::qi;

struct ImageContext : MockContext
{
    ImageContext(const bool flag)
        : m_flag { flag }
    {}

    bool fromImageTag() const override { return m_flag; }

    bool m_flag;
};

const static ImageOption option { false };
const static ImageOption optionInv { true };

TEST(ImageOption, Constructor)
{
    EXPECT_FALSE(option.inverse());
    EXPECT_TRUE(optionInv.inverse());
}

TEST(ImageOption, ImageContext)
{
    const Uri uri { "http://adblock.org/image.jpg" };
    const ImageContext context { true };

    EXPECT_TRUE(option.match(uri, context));
}

TEST(ImageOption, NotImageContext)
{
    const Uri uri { "http://adblock.org/script.js" };
    const ImageContext context { false };

    EXPECT_FALSE(option.match(uri, context));
}

TEST(ImageOption, ImageContextWithInverseOption)
{
    const Uri uri { "http://adblock.org/image.jpg" };
    const ImageContext context { true };

    EXPECT_FALSE(optionInv.match(uri, context));
}

TEST(ImageOption, NotImageContextWithInverseOption)
{
    const Uri uri { "http://adblock.org/script.js" };
    const ImageContext context { false };

    EXPECT_TRUE(optionInv.match(uri, context));
}

} // namespace adblock
