#include "option/xml_http_request_option.hpp"
#include "../mock_context.hpp"

#include <gtest/gtest.h>

namespace adblock {

struct XmlHttpRequestContext : MockContext
{
    XmlHttpRequestContext(const bool flag)
        : m_flag { flag }
    {}

    bool fromXmlHttpRequest() const override { return m_flag; }

    bool m_flag;
};

const static XmlHttpRequestOption option { false };
const static XmlHttpRequestOption optionInv { true };

TEST(Option_XmlHttpRequestOption, Constructor)
{
    EXPECT_FALSE(option.inverse());
    EXPECT_TRUE(optionInv.inverse());
}

TEST(Option_XmlHttpRequestOption, XmlHttpRequestContext)
{
    const Uri uri { "http://adblock.org/image.jpg" };
    const XmlHttpRequestContext context { true };

    EXPECT_TRUE(option.match(uri, context));
}

TEST(Option_XmlHttpRequestOption, NotXmlHttpRequestContext)
{
    const Uri uri { "http://adblock.org/script.js" };
    const XmlHttpRequestContext context { false };

    EXPECT_FALSE(option.match(uri, context));
}

TEST(Option_XmlHttpRequestOption, XmlHttpRequestContextWithInverseOption)
{
    const Uri uri { "http://adblock.org/image.jpg" };
    const XmlHttpRequestContext context { true };

    EXPECT_FALSE(optionInv.match(uri, context));
}

TEST(Option_XmlHttpRequestOption, NotXmlHttpRequestContextWithInverseOption)
{
    const Uri uri { "http://adblock.org/script.js" };
    const XmlHttpRequestContext context { false };

    EXPECT_TRUE(optionInv.match(uri, context));
}

} // namespace adblock
