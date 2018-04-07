#include "option/object_subrequest_option.hpp"
#include "../mock_context.hpp"

#include <boost/spirit/include/qi.hpp>

#include <gtest/gtest.h>

namespace adblock {

namespace qi = boost::spirit::qi;

struct ObjectSubRequestContext : MockContext
{
    ObjectSubRequestContext(const bool flag)
        : m_flag { flag }
    {}

    bool fromPlugins() const override { return m_flag; }

    bool m_flag;
};

const static ObjectSubRequestOption option { false };
const static ObjectSubRequestOption optionInv { true };

TEST(ObjectSubRequestOption, Constructor)
{
    EXPECT_FALSE(option.inverse());
    EXPECT_TRUE(optionInv.inverse());
}

TEST(ObjectSubRequestOption, ObjectSubRequestContext)
{
    const Uri uri { "http://adblock.org/image.jpg" };
    const ObjectSubRequestContext context { true };

    EXPECT_TRUE(option.match(uri, context));
}

TEST(ObjectSubRequestOption, NotObjectSubRequestContext)
{
    const Uri uri { "http://adblock.org/script.js" };
    const ObjectSubRequestContext context { false };

    EXPECT_FALSE(option.match(uri, context));
}

TEST(ObjectSubRequestOption, ObjectSubRequestContextWithInverseOption)
{
    const Uri uri { "http://adblock.org/image.jpg" };
    const ObjectSubRequestContext context { true };

    EXPECT_FALSE(optionInv.match(uri, context));
}

TEST(ObjectSubRequestOption, NotObjectSubRequestContextWithInverseOption)
{
    const Uri uri { "http://adblock.org/script.js" };
    const ObjectSubRequestContext context { false };

    EXPECT_TRUE(optionInv.match(uri, context));
}

} // namespace adblock
