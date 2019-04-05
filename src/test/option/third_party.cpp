#include "option/third_party_option.hpp"
#include "../mock_context.hpp"

#include <gtest/gtest.h>

namespace adblock {

struct ThirdPartyContext : MockContext
{
    ThirdPartyContext(const Uri &uri)
        : m_uri { uri }
    {}

    const Uri &origin() const override { return m_uri; }

    const Uri &m_uri;
};

const static ThirdPartyOption option { false };
const static ThirdPartyOption optionInv { true };

TEST(ThirdPartyOption, Constructor)
{
    EXPECT_FALSE(option.inverse());
    EXPECT_TRUE(optionInv.inverse());
}

TEST(ThirdPartyOption, ThirdPartyContext)
{
    const Uri uri { "http://adblock.org/image.jpg" };
    const Uri origin { "http://adblock.org" };
    const ThirdPartyContext context { origin };

    EXPECT_FALSE(option.match(uri, context)) << uri.host() << "\n"
                                            << context.origin().host();
}

TEST(ThirdPartyOption, NotThirdPartyContext)
{
    const Uri uri { "http://adblock.org/script.js" };
    const Uri origin { "http://www.google.com" };
    const ThirdPartyContext context { origin };

    EXPECT_TRUE(option.match(uri, context));
}

TEST(ThirdPartyOption, NotThirdPartyContextWithInverseOption)
{
    const Uri uri { "http://adblock.org/image.jpg" };
    const Uri origin { "http://adblock.org" };
    const ThirdPartyContext context { origin };

    EXPECT_TRUE(optionInv.match(uri, context));
}

TEST(ThirdPartyOption, ThirdPartyContextWithInverseOption)
{
    const Uri uri { "http://adblock.org/script.js" };
    const Uri origin { "http://www.google.com" };
    const ThirdPartyContext context { origin };

    EXPECT_FALSE(optionInv.match(uri, context));
}

TEST(ThirdPartyOption, PolymorphicInverseOption)
{
    const Uri uri { "http://adblock.org/script.js" };
    const Uri origin { "http://www.google.com" };
    const ThirdPartyContext context { origin };

    const auto *ptr = dynamic_cast<const Option*>(&optionInv);
    assert(ptr);

    EXPECT_FALSE(ptr->match(uri, context));
}

} // namespace adblock
